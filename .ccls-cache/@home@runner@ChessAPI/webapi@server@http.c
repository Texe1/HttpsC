#include "http.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>

string HTTP_METHOD_NAMES[9] = {
    STR_OF("GET"),     STR_OF("HEAD"),   STR_OF("POST"),
    STR_OF("PUT"),     STR_OF("DELETE"), STR_OF("CONNECT"),
    STR_OF("OPTIONS"), STR_OF("TRACE"),  STR_OF("PATCH"),
};

#define MAX_PATH_LENGTH 2048

// TODO implement
void *http_rcv_cb(struct server *srv, uint idx) {
  struct serverside_client_data *client = &(srv->clients[idx]);

  uint method;

  char buf[129];
  int n = 0;
  int i = 0;

  // Method
  {
    n = recv(client->fd, buf, 128, 0);

    for (method = 0; method <= PATCH; method++) {
      string *s = HTTP_METHOD_NAMES + method;
      if (n >= s->len && !memcmp(s->c, buf, s->len)) {
        i = s->len + 1;
        break;
      }
    }

    printf("method = %s(%d)\n", HTTP_METHOD_NAMES[method].c, method);
  }

  char *path;

  uint path_len = 0;

  // Path
  {

    char tmp[MAX_PATH_LENGTH + 1];

    memcpy(tmp, buf + i, 128 - i);

    for (; path_len + i < 128 && tmp[path_len] != ' '; path_len++)
      ;

    while (tmp[path_len] != ' ') {
      if (path_len == MAX_PATH_LENGTH)
        return NULL;
      if (recv(client->fd, tmp + path_len, 1, 0) <= 0) {
        return NULL;
      }
      path_len++;
    }
    path = malloc(path_len + 1);
    memcpy(path, tmp, path_len);
    path[path_len] = 0;
    printf("Path(length %d): %s\n", path_len, path);
  }

  // TMP
  {

    while (n > 0) {
      buf[n] = 0;
      printf("%s", buf);
      n = recv(client->fd, buf, 128, 0);
    }
    char response[] =
        "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: "
        "22\r\n\r\n<h1>Hello, World!</h1>";

    send(client->fd, response, strlen(response), 0);

    close(client->fd);
    client->valid = 0;
  }

  return NULL;
}

// TODO immplement
struct server start_std_http_server(ushort port) {
  struct server srv = {
      .port = port,
      .flags = 0,
      .rcv = http_rcv_cb,
      .clients = NULL,
      .nClients = 0,
      .first_emtpy_client = 0,
  };

  return server_connect(srv);
}