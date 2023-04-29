#include "header_types.h"

uint search_http_header_type_idx(string s, uint start, uint end) {
  if (end - start <= 1) {
    if (HTTP_HEADER_TYPE_STRINGS[start].len == s.len &&
        !memcmp(HTTP_HEADER_TYPE_STRINGS[start].c, s.c, s.len)) {
      return start;
    }
  } else {
    int mid = (start + end) / 2;
    int res = search_http_header_type_idx(s, start, mid);
    if (res != -1)
      return res;
    else
      return search_http_header_type_idx(s, mid, end);
  }
  return -1;
}

uint get_http_header_type_idx(string s) {
  return search_http_header_type_idx(s, 0, N_HEADER_TYPES);
}