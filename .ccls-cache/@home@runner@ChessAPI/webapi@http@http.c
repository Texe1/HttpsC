#include "http.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/time.h>

#include "http_parser.h"

#include "../../util.h"

struct string http_response(int client_idx, struct string msg,
                            struct server_data *data) {

  struct timeval begin, end;
	printf("Raw Message:\n%s\nEND.", msg.c);
  gettimeofday(&begin, NULL);

	printf("0\n");
  struct http_request req = parse_request(msg);

	printf("1\n");

	
  gettimeofday(&end, NULL);

  print_http_req(req);

  long micros =
      end.tv_usec - begin.tv_usec + 1000000 * (end.tv_sec - begin.tv_sec);

  printf("\\\\This request was parsed in %ld µs\n", micros);

  const char *c =
      "HTTP/1.1 200 Ok\r\nContent-Type: text/html\r\nContent-Length: "
      "13\r\n\r\n<h1>TEST</h1>";
  /*{
    print_http_req(req);
    if (req.body.c)
      free(req.body.c);

    if (req.path.allocated && req.path.c)
      free(req.path.c);
    for (int i = 0; i < req.header.n; i++) {
      struct http_req_header_field *field = req.header.fields + i;
      if (!field)
        continue;
      for (int j = 0; j < field->nVals; j++) {
        struct http_req_header_val_group *group = field->val_groups + i;
        if (!group)
          continue;
        for (int k = 0; k < group->n; k++) {
          struct http_req_header_value *val = group->vals;
          if (val->val)
            free(val->val);
          if (val->key)
            free(val->key);
        }
        free(group->vals);
      }
      free(field->val_groups);
    }
    free(req.header.fields);
  }*/
  return (struct string){.c = (char *)c,
                         .len = strlen(c),
                         .allocated = 0,
                         .null_terminated = 1,
                         .EOF_terminated = 0};
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