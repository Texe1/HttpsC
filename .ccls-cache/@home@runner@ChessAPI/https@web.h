#include <sys/socket.h>

enum CONNECTION_TYPE {
  RAW,
  HTTP_SERVER,  // implementing not finished
  HTTPS_SERVER, // not implemented
  HTTP_CLIENT,  // not implemented
  HTTPS_CLIENT, // not implemented
};

struct connection;

typedef char *(response)(char *, struct connection *, int);

struct connection {
  // the port on which the connection is opened
  short port;
  // the socket for the connection
  int socket;

  // if this even is a connection (all 1's)
	ushort active;

  pthread_t main_thread;

  response respond;

  int nClient_pools;
  struct client_thread_data {
    pthread_t thread;
    uint nClients;
    int *client_sockets;
  } * client_thread_data;
};

int start_connection(struct connection conn);
int stop_connection(struct connection conn);