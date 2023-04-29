#include <stdio.h>

#include <signal.h>
#include <string.h>
#include <unistd.h>

#include "./webapi/server/http.h"
#include "./webapi/server/server.h"

void *rcv_test(struct server *srv, uint idx);

struct server srv;

void termination_handler(int sig) {

  terminate_server(&srv);
  printf("Hello!\n");
}

int main(void) {

  srv = start_std_http_server(80);

  signal(SIGTERM, termination_handler);
  signal(SIGINT, termination_handler);
  loop_server(&srv);

  close(srv.port);
  close(srv.epoll);

  return 0;
}

void *rcv_test(struct server *srv, uint idx) {
  struct serverside_client_data *client = &(srv->clients[idx]);

  // TMP
  {

    char buf[129];
    int n = 0;

    while ((n = recv(client->fd, buf, 128, 0)) > 0) {
      buf[n] = 0;

      printf("%s", buf);
    }
    char response[] =
        "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: "
        "22\r\n\r\n<h1>Hello, World!</h1>";

    send(client->fd, response, strlen(response), 0);

    close(client->fd);
  }

  return NULL;
}