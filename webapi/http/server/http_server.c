#include "http_server.h"

#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>

#include <signal.h>
#include <stdio.h>
#include <string.h>

#include "../http1/construct.h"
#include "../http1/parse.h"

int srv_add_client(HTTP_SERVER *srv, uint clientfd, struct sockaddr *addr) {
  printf("new Client! currently %d clients\n", srv->n_conn);
  if (!srv->clients) {
    srv->clients = malloc(sizeof(struct http_srv_conn));
    srv->clients[0] = (struct http_srv_conn){
        .fd = clientfd,
        .addr = *addr,
        .present = 1,
        .version = HTTP1_1,
    };
    srv->n_conn = 1;
    return 0;
  }

  for (uint idx = 0; idx < srv->n_conn; idx++) {
    if (srv->clients[idx].present == 0) {
      srv->clients[idx] = (struct http_srv_conn){
          .fd = clientfd,
          .addr = *addr,
          .present = 1,
          .version = HTTP1_1,
      };
      return idx;
    }
  }

  uint new_sz = sizeof(struct http_srv_conn) * (srv->n_conn + 1);
  void *tmp = realloc(srv->clients, new_sz);
  if (!tmp) {
    close(clientfd);
    return -1;
  }
  srv->clients = tmp;
  uint idx = srv->n_conn;
  srv->n_conn++;
  srv->clients[idx] = (struct http_srv_conn){
      .fd = clientfd,
      .addr = *addr,
      .present = 1,
      .version = HTTP1_1,
  };
  return idx;
}

struct http_srv_conn *srv_get_conn_from_idx(HTTP_SERVER *srv, uint idx) {
  if (idx >= srv->n_conn)
    return NULL;
  return srv->clients + idx;
}

int http_respond(HTTP_SERVER *srv, struct http_srv_conn *conn, char *in_data) {
  HTTP_REQUEST req = parse_http_1_request(in_data);
  HTTP_RESPONSE res;

  int i = srv->cb(&req, &res);

  switch (i) {
  case SCB_CLS:
    conn->close = 1;
    break;
  case SCB_DGR:
    if (conn->version > 0)
      conn->version--;
    break;
  case SCB_UGR:
    if (conn->version < HTTP3_0) {
      conn->version++;
    }
    break;
  default:
    break;
  }

  return 0;
}

void *server_loop(void *data) {
  HTTP_SERVER *srv = (HTTP_SERVER *)data;

  struct epoll_event evts[8];

  while (1) {
    int n = epoll_wait(srv->epoll, evts, 8, 1000);

    if (n > 0) {
      for (uint i = 0; i < n; i++) {
        struct epoll_event *evt = evts + i;

        uint idx = evt->data.u32;

        if (idx == 0) { // server socket
          struct sockaddr client_addr;
          uint addrlen = 0;
          uint client_fd = accept(srv->socket, &client_addr, &addrlen);

          int i = srv_add_client(srv, client_fd, &client_addr);
          if (i >= 0) {
            struct epoll_event evt = {
                .data.u32 = i + 1,
                .events = EPOLLIN,
            };
            epoll_ctl(srv->epoll, EPOLL_CTL_ADD, client_fd, &evt);
          }
        } else {

          struct http_srv_conn *conn = srv_get_conn_from_idx(srv, idx - 1);

          if (!conn || !conn->present)
            continue;

          printf("trying to read from socket %d\n", conn->fd);

          HTTP_REQUEST req;
          HTTP_RESPONSE res;

          // TODO real reading to malloced ptr
          char *buf = malloc(65);
          uint n_read = 0;
          uint len = 0;
          while (1) {
            n_read = recv(conn->fd, buf + len, 64, 0);
            len += n_read;
            if (n_read == 64) {
              buf = realloc(buf, len + 65);
              continue;
            }
          }

          buf[len] = 0;
          req = parse_http_1_request(buf);

          free(buf);
          srv->cb(&req, &res);

          // TODO change to custom string struct, with length and if malloced
          // 
          {
            if (req.body)
              free(req.body);
            if (req.path)
              free(req.path);
            for (uint i = 0; i < NUM_HTTP_REQ_HEADERS; i++) {
              if (req.header[i])
                free(req.header[i]);
            }
          }

          char *res_str = http1_construct_response(&res);

          printf("trying to write to socket %d\n", conn->fd);
          send(conn->fd, res_str, strlen(res_str), 0);

          // Only test, remove
          {
            conn->close = 1;
            if (conn->close) {
              conn->present = 0;
              epoll_ctl(srv->epoll, EPOLL_CTL_DEL, conn->fd, NULL);
              close(conn->fd);
            }
          }
        }
      }
    }
  }

  return NULL;
}

HTTP_SERVER *http_create_server(HTTP_SERVER_SETTINGS *cfg) {

  HTTP_SERVER *srv = malloc(sizeof(HTTP_SERVER));

  *srv = (HTTP_SERVER){0};
  srv->config = *cfg;
  srv->cb = cfg->cb;

  struct sockaddr_in inaddr = {
      .sin_family = AF_INET,
      .sin_port = htons(cfg->port),
      .sin_zero = 0,
      .sin_addr.s_addr = INADDR_ANY,
  };

  srv->socket = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);

  setsockopt(srv->socket, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int));

  bind(srv->socket, (struct sockaddr *)&inaddr, sizeof(inaddr));

  listen(srv->socket, 64);

  srv->epoll = epoll_create1(0);

  struct epoll_event evt = {
      .data.u32 = 0,
      .events = EPOLLIN,
  };

  epoll_ctl(srv->epoll, EPOLL_CTL_ADD, srv->socket, &evt);

  return srv;
}
