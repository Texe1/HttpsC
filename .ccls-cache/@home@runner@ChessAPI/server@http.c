#include "http.h"

char *http_wrapper(char *in, struct connection *conn, int client) {
  return default_http_Response(in, (void *)0);
}

char *default_http_Response(char *req, void *data) {
  return "HTTP/1.1 200 Ok\r\nContent-Length: 28\r\nContent-Type: text/html\r\n\r\n<h1>AAAAAAAAAAAAAAAAAAA</h1>";
}