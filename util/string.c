#include "string.h"

#include <stdlib.h>
#include <string.h>

inline void delete_str(STRING *s) {
  if (s->malloced) {
    free(s->c);
    *s = (STRING){0};
  }
}

inline STRING malloc_str(uint sz) {
  char *ptr = malloc(sz + 1);

  if (!ptr) {
    return (STRING){0};
  }

  return (STRING){
      .len = sz,
      .malloced = 1,
      .null_terminated = 1,
      .c = ptr,
  };
}

inline STRING str_from_char_ptr(char *c) {
  STRING s = {
      .len = strlen(c),
      .malloced = 1,
      .null_terminated = 1,
  };
  s.c = malloc(s.len + 1);

  memcpy(s.c, c, s.len + 1);

  return s;
}

inline STRING str_from_char_ptr_fixed_len(char *c, uint len) {
  char *tmp = malloc(len + 1);
  if (!tmp) {
    return (STRING){0};
  }
  return (STRING){
      .len = len,
      .c = tmp,
      .malloced = 1,
      .null_terminated = 1,
  };
}

inline void append_str(STRING *s0, STRING *s1) {
  char *dst = malloc(s0->len + s1->len + 1);
  memcpy(dst, s0->c, s0->len);
  memcpy(dst + s0->len, s1->c, s1->len);
  s0->len += s1->len;
  dst[s0->len] = 0;
  if (s0->malloced) {
    free(s0->c);
  }
  s0->malloced = 1;
  s0->null_terminated = 1;
  s0->c = dst;
}