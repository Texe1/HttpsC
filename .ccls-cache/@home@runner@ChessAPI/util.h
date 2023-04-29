#pragma once

struct string {
  unsigned long len;
  char *c;
  unsigned char allocated : 1, null_terminated : 1
};