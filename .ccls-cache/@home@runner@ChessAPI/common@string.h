#pragma once

#include <stdlib.h>
#include <string.h>

typedef struct str {
  char *c;
  ulong len;
} string;

#define STR_OF(s)                                                              \
  (string) { (char *)s, strlen(s) }