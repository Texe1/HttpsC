#include "http.h"

#include <stdlib.h>

#define HTTP_PORT 8080

char *http_response(int client_idx, char *msg, struct server_data *data) {
  return "HTTP/1.1 200 Ok\r\nContent-Type: text/html\r\nContent-Length: "
         "13\r\n\r\n<h1>TEST</h1>";
}

pthread_t start_http_server(short port, struct http_server_data http_data) {
  void *tmp =
      malloc(sizeof(struct server_data) + sizeof(struct http_server_data));
  if (!tmp) {
    return (pthread_t)NULL;
  }

  struct server_data *ptr_data = tmp;
  *ptr_data = (struct server_data){.port = port,
                                   .clients = NULL,
                                   .nClients = 0,
                                   .respond = http_response,
                                   .data = tmp + sizeof(struct server_data)};

  *((struct http_server_data *)ptr_data->data) = http_data;

  pthread_t res;
  pthread_create(&res, NULL, (void *(*)(void *))server_thread,
                 (void *)ptr_data);

  return res;
}