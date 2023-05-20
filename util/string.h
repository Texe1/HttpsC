#pragma once
#include "typedef.h"

typedef struct string {
  uint len : 32, malloced : 1, null_terminated : 1;
  char *c;
} STRING;

#define CONST_STR(s)                                                           \
  (STRING) {                                                                   \
    .len = strlen(s), .malloced = 0, .null_terminated = 1, .c = (char *)(s)    \
  }

// delete [s], if allocated
inline void delete_str(STRING *s);
// heap-allocate a NULL-terminated string of length sz (+1 for NULL-Terminator)
inline STRING malloc_str(uint sz);
// append s1 to s0, results in a heap-allocated, NULL-terminated string
inline void append_str(STRING *s0, STRING *s1);
// creates a string object with a copy of [c] (auto-detect length)
inline STRING str_from_char_ptr(char *);
// creates a string object with a copy of [c] and length len
inline STRING str_from_char_ptr_fixed_len(char *c, uint len);