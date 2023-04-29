#include "web.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <errno.h>
#include <fcntl.h>
#include <signal.h>

void *server_thread(struct server_data *data) {

  printf("starting server thread...\n");

  // ignoring signal "broken pipe", instead handling it manually as error
  sigaction(SIGPIPE, &(struct sigaction){SIG_IGN}, NULL);

  data->nClients = 0;
  data->clients = NULL;

  int sock = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);

  printf("a\n");
  if (sock == -1)
    return NULL;
  printf("a\n");

  int optval;

  struct sockaddr_in addr = {.sin_family = AF_INET,
                             .sin_port = htons(data->port),
                             .sin_addr.s_addr = INADDR_ANY};

  int result;

  if ((result = bind(sock, (struct sockaddr *)&addr, sizeof(addr))) == -1) {
    printf("b\n");
    close(sock);
    return NULL;
  }

  if ((result = listen(sock, 8)) == -1) {
    close(sock);
    return NULL;
  }
  printf("a\n");

  printf("main loop started...\n");
  for (;;) {

    usleep(1000);

    struct sockaddr client_addr;
    socklen_t addr_sz;
    int client = accept(sock, &client_addr, &addr_sz);

    if (client != -1) {

      int flags = fcntl(client, F_SETFL);
      if (flags == -1) {
        close(client);
        continue;
      }

      fcntl(client, F_SETFL, flags | O_NONBLOCK);

      int idx = -1;

      for (int i = 0; i < data->nClients; i++) {
        if (data->clients[i].socket == -1) { // space for new connection
          idx = i;
          break;
        }
      }

      if (idx < 0) {
        // memory allocation (might break outer loop, ending this function)
        {
          void *tmp;
          if (data->nClients == 0) {
            tmp = malloc(8 * sizeof(struct client_data));
          } else if (data->nClients % 8 == 0) {
            tmp = realloc(data->clients,
                          sizeof(struct client_data) * (data->nClients + 8));
          }

          if (!tmp) {
            close(client);
            break;
          }

          data->clients = tmp;
        }
        idx = data->nClients;
        data->nClients++;
      }

      data->clients[idx] =
          (struct client_data){.socket = client, .addr = client_addr};
    }

    if (data->clients) { // there are clients

      // looping through all clients
      for (int i = 0; i < data->nClients; i++) {
        int client_sock = data->clients[i].socket;

        if (client_sock == -1) // not valid
          continue;

        char *buf = malloc(128);
        if (!buf) // allocation will likely not work when tried again instantly
          break;

        // testing read();
        int result = read(client_sock, buf, 128);
        if (result == -1) {
          if (errno != EWOULDBLOCK &&
              errno != EAGAIN) { // some error, dropping connection
            close(client_sock);
            data->clients[i].socket = -1;
          }
          continue;
        }

        //  additional bytes
        int n = 0;
        while (result == 128) {
          n += 128;
          buf = realloc(buf, n + 128);
          if (!buf)
            break; // realloc returned zero
          result = read(client_sock, buf + n, 128);
        }

        if (result >= 0)
          n += result;
        buf[n] = 0;

        struct string s = {.c = buf, .len = n};

        // formulating and writing a response
        struct string response = data->respond(client_sock, s, data);
        printf("REPLYING TO: \n\n%s\n\nWITH:\n\n%s\n\nEND.\n\n", buf,
               response.c);

        free(buf);

        result = write(client_sock, response.c, response.len);

        if (response.allocated)
          free(response.c);

        if (result == -1) {
          close(client_sock);
          data->clients[i].socket = -1;
        }
      }
    }
  }

  close(sock);
  return NULL;
}