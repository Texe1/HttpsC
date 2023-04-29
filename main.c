#include <stdio.h>
#include <stdlib.h>

#include <sys/time.h>

#include "webapi/http/request.h"

const char *test_req = "GET /index.html HTTP/3\r\n"
                       "Host : www.google.com\r\n"
                       "User-Agent : Mozilla / 5.0(Windows NT 10.0; Win64; "
                       "x64; rv : 109.0)Gecko / 20100101 Firefox / 112.0\r\n"
                       "Accept : */*;q=0.9\r\n"
                       "Accept : text/html\r\n"
                       "Accept-Language: en-US,en;q=0.5\r\n"
                       "Accept-Encoding: gzip, deflate, br\r\n"
                       "Referer: https://www.google.com/\r\n"
                       "Alt-Used: www.google.com\r\n"
                       "Connection: keep-alive\r\n"
                       "Sec-Fetch-Dest: script\r\n"
                       "Sec-Fetch-Mode: no-cors\r\n"
                       "Sec-Fetch-Site: same-origin\r\n\r\n";

const char *post_test =
    "POST /echo/post/json HTTP/1.1\r\n"
    "Authorization: Bearer mt0dgHmLJMVQhvjpNXDyA83vA_Pxh33Y\r\n"
    "Accept: application/json\r\n"
    "Content-Type : application/json\r\n"
    "Content-Length : 80\r\n"
    "Host : reqbin.com\r\n\r\n"
    "{\n"
    " \"Id\" : 12345,\n"
    " \"Customer\" : \"John Smith\",\n"
    " \"Quantity\" : 1,\n"
    " \"Price\" : 10.00\n"
    "}";

int main(void) {
  struct timeval start, end;
#define NUM_PASSES 1
  HTTP_REQUEST req = {0};

  gettimeofday(&start, NULL);
  for (uint i = 0; i < NUM_PASSES; i++) {
    if (req.path)
      free(req.path);
    if (req.body)
      free(req.body);
    req = parseRequest((char *)post_test);
  }
  gettimeofday(&end, NULL);

  long diff =
      (end.tv_sec - start.tv_sec) * 1000000 + end.tv_usec - start.tv_usec;

  printRequest(req);
  if (req.path)
    free(req.path);
  if (req.body)
    free(req.body);
  return 0;

  return 0;
}