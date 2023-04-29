#include "http_parser.h"

#include <string.h>

#include <stdio.h>
#include <unistd.h>

const char *HTTP_REQ_HEADER_TYPE_STRINGS[] = {"A-IM",
                                              "Accept",
                                              "Accept-Charset",
                                              "Accept-Datetime",
                                              "Accept-Encoding",
                                              "Accept-Language",
                                              "Access-Control-Request-Method",
                                              "Access-Control-Request-Headers",
                                              "Authorization",
                                              "Cache-Control",
                                              "Connection",
                                              "Content-Encoding",
                                              "Content-Length",
                                              "Content-MD5",
                                              "Content-Type",
                                              "Cookie",
                                              "Date",
                                              "Expect",
                                              "Forwarded",
                                              "From",
                                              "Host",
                                              "HTTP2-Settings",
                                              "If-Match",
                                              "If-Modified-Since",
                                              "If-None_Match"
                                              "If-Range",
                                              "If-Unmodified-Since",
                                              "Max-Forwards",
                                              "Origin",
                                              "Pragma",
                                              "Prefer",
                                              "Proxy-Authorization",
                                              "Range",
                                              "Referer",
                                              "TE",
                                              "Trailer",
                                              "Transfer-Encoding",
                                              "User-Agent",
                                              "Upgrade",
                                              "Via",
                                              "Warning"};

#define NUM_HTTP_REQ_HEADER_TYPES                                              \
  sizeof(HTTP_REQ_HEADER_TYPE_STRINGS) / sizeof(const char *)

// fast lookup so we don't have to check the whole array of type strings
u_char HTTP_REQ_HEADER_TYPE_LOOKUP[] = {
    A_IM,         CACHE_CONTROL, CACHE_CONTROL, DATE,     EXPECT,
    FORWARDED,    HOST,          HOST,          IF_MATCH, MAX_FORWARDS,
    MAX_FORWARDS, MAX_FORWARDS,  MAX_FORWARDS,  ORIGIN,   ORIGIN,
    PRAGMA,       REFERER,       REFERER,       TE,       TE,
    USER_AGENT,   VIA,           WARNING};

const char *HTTP_METHOD_STRINGS[] = {"GET",     "HEAD",   "POST",
                                     "PUT",     "DELETE", "CONNECT",
                                     "OPTIONS", "TRACE",  "PATCH"};

const char *get_http_method_string(int method) {
  if ((uint)method > sizeof(HTTP_METHOD_STRINGS) / sizeof(const char *)) {
    return NULL;
  }
  return HTTP_METHOD_STRINGS[method];
}

int get_header_type_idx(char *c, int len) {
  if (!c || len == 0 || c[0] < 'A' ||
      c[0] >= sizeof(HTTP_REQ_HEADER_TYPE_LOOKUP) + 'A')
    return -1;

  uint from = HTTP_REQ_HEADER_TYPE_LOOKUP[c[0] - 'A'];
  uint to = (c[0] == sizeof(HTTP_REQ_HEADER_TYPE_LOOKUP) + 'A' - 1)
                ? WARNING + 1
                : HTTP_REQ_HEADER_TYPE_LOOKUP[c[0] - 'A' + 1];

  for (int i = from; i < to; i++) {
    const char *s = HTTP_REQ_HEADER_TYPE_STRINGS[i];
    if (strlen(s) == len && !memcmp(c, s, len)) {
      return i;
    }
  }

  return -1;
}

struct http_request parse_request(struct string str) {
  struct http_request req = {0};

  int idx = 0;

  // method
  {
    req.method = -1;
    for (int i = 0; i < sizeof(HTTP_METHOD_STRINGS) / sizeof(char *); i++) {
      const char *s = HTTP_METHOD_STRINGS[i];
      if (!memcmp(str.c, s, strlen(s))) {
        req.method = i;
        idx += strlen(s);
        break;
      }
    }

    if (req.method == -1 || str.c[idx] != ' ')
      return (struct http_request){.method = -1};

    idx++;
  }

  // path
  {
    for (int i = idx; i < str.len - 1; i++) {
      if (str.c[i] == ' ') {
        req.path = (struct string){.c = malloc(i - idx + 1),
                                   .len = i - idx,
                                   .allocated = 1,
                                   .null_terminated = 1,
                                   .EOF_terminated = 0};
        memcpy(req.path.c, str.c + idx, i - idx);
        req.path.c[i - idx] = 0;

        idx = i;
        break;
      }
    }

    if (!req.path.c || str.c[idx] != ' ')
      return (struct http_request){.method = -1};

    idx++;
  }

  // HTTP version (1.0, 1.1, 2.0)
  {
    if (str.len < idx + 10)
      return (struct http_request){.method = -1};
    if (!memcmp(str.c + idx, "HTTP/1.1\r\n", 10))
      req.version = HTTP_1_1;
    else if (!memcmp(str.c + idx, "HTTP/2.0\r\n", 10))
      req.version = HTTP_2_0;
    else if (!memcmp(str.c + idx, "HTTP/1.1\r\n", 10))
      req.version = HTTP_2_0;
    else
      return (struct http_request){.method = -1};

    idx += 10;
  }

  // header
  {
    if (str.c[idx] == '\r' && str.c[idx + 1] == '\n')
      return req;

    for (int i = idx; i < str.len - 1; i++) {
      if (str.c[i] == '\r' && str.c[i + 1] == '\n') {
        uint type_len = -1;

        for (int j = idx; j < i; j++) {
          if (str.c[j] == ':') {
            type_len = j - idx;
            break;
          }
        }

        uint type_idx = get_header_type_idx(str.c + idx, type_len);
        if (type_idx >= 0 && type_idx <= WARNING) {
          struct http_header_line *line = req.header + type_idx;

          {
            idx += type_len + 1;
            while (str.c[idx] == ' ') {
              idx++;
            }
          }
          uint start = idx;

          // counting amount of value groups
          uint n = 0;
          {
            char empty = 1;
            while (idx < i) {
              idx++;
              if (str.c[idx] == ',') {
                if (!empty) {
                  n++;
                  empty = 1;
                }
              } else if (str.c[idx] != ' ') {
                empty = 0;
              }
            }
            if (!empty)
              n++;

            line->value_groups = malloc(n * sizeof(struct http_header_values));
          }

          // counting the values in each group
          {
            char empty = 1;
            uint nVals = 0;
            for (int j = start; j < i; j++) {
              switch (str.c[j]) {
              case ';':
                if (!empty) {
                  nVals++;
                  empty = 1;
                }
                break;
              case ',': {

                if (!empty) {
                  nVals++;
                  empty = 1;
                }
                if (nVals > 0) {

                  struct http_header_values *vals =
                      line->value_groups + line->n;
                  vals->keys = calloc(sizeof(char *), nVals);
                  vals->vals = calloc(sizeof(char *), nVals);
                  vals->n = nVals;
                  line->n++;
                }
                nVals = 0;
                break;
              }
              case ' ':
                break;
              default:
                empty = 0;
                break;
              }
            }

            if (!empty)
              nVals++;
            if (nVals > 0) {

              struct http_header_values *vals = line->value_groups + line->n;
              vals->keys = calloc(nVals, sizeof(char *));
              vals->vals = calloc(nVals, sizeof(char *));

              vals->n = nVals;
              line->n++;
            }
          }

          // filling in the values
          {

            uint n_groups = 0, n_vals = 0;

            char empty = 1;

            uint j = start;
            for (; j < i; j++) {
              char c = str.c[j];
              if (!empty && (c == ',' || c == ';' || c == '=')) {
                idx = j;
                while (str.c[idx - 1] == ' ')
                  idx--;

                int len = idx - start;

                char *tmp = malloc(len + 1);
                memcpy(tmp, str.c + start, len);
                tmp[len] = 0;

                j++;
                while (str.c[j] == ' ')
                  j++;
                start = j;
                j--;

                if (c == '=') {
                  line->value_groups[n_groups].keys[n_vals] = tmp;
                } else {
                  line->value_groups[n_groups].vals[n_vals] = tmp;
                  if (c == ';') {
                    n_vals++;
                  } else {
                    n_vals = 0;
                    n_groups++;
                  }
                }

              } else if (c != ' ') {
                if (empty) {
                  empty = 0;
                  while (str.c[j] == ' ')
                    j++;
                  start = j;
                  j--;
                }
              }
            }

            if (!empty) {
              idx = j;
              while (str.c[idx - 1] == ' ')
                idx--;

              int len = idx - start;

              char *tmp = malloc(len + 1);
              memcpy(tmp, str.c + start, len);
              tmp[len] = 0;

              line->value_groups[n_groups].vals[n_vals] = tmp;
            }
          }
        }

        idx = i + 2;
        if (str.c[i + 2] == '\r' && str.c[i + 3] == '\n') {
          idx += 2;
          break;
        }
      }
    }
    if (req.header[CONTENT_LENGTH].n != 1 ||
        req.header[CONTENT_LENGTH].value_groups[0].n != 1 ||
        req.header[CONTENT_LENGTH].value_groups[0].keys[0]) {
      return (struct http_request){.method = -1};
    }

    char *c = req.header[CONTENT_LENGTH].value_groups[0].vals[0];

    long l = 0;

    for (int i = 0; c[i] != 0; i++) {
      if (c[i] < '0' || c[i] > '9')
        return (struct http_request){.method = -1};

      l = 10 * l + c[i] - '0';
    }

    if (l != 0) {

      req.body = (struct string){.c = malloc(l + 1),
                                 .len = l,
                                 .allocated = 1,
                                 .null_terminated = 1,
                                 .EOF_terminated = 0};

      memcpy(req.body.c, str.c + idx, l);
      req.body.c[l] = 0;
    }
  }

  return req;
}

void print_http_req(struct http_request r) {
  if (r.method == -1) {
    return;
  }

  const char *str = HTTP_METHOD_STRINGS[r.method];
  printf("%s %s HTTP/%c.%c\n", str, r.path.c,
         (r.version == HTTP_2_0) ? '2' : '1',
         (r.version == HTTP_1_1) ? '1' : '0');

  for (int i = 0; i < WARNING + 1; i++) {
    struct http_header_line *line = r.header + i;

    if (line->value_groups && line->n != 0) {
      printf("%d:\n", i);
      printf(" %s: ", HTTP_REQ_HEADER_TYPE_STRINGS[i]);
      for (int j = 0; j < line->n; j++) {
        struct http_header_values *vals = line->value_groups + j;

        for (int k = 0; k < vals->n; k++) {
          if (vals->keys[k])
            printf("%s = ", vals->keys[k]);
          printf("%s", vals->vals[k]);
          if (k < vals->n - 1)
            printf("; ");
        }
        if (j < line->n)
          printf(", ");
      }
      putchar('\n');
    }
  }

  if (r.body.c && r.body.len > 0) {
    printf("\r\n%s", r.body.c);
  }
}

// void free_http_req(struct http_request);