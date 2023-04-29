#include "util.h"

#include <fcntl.h>

uint set_non_block(uint fd) {
  int opt = fcntl(fd, F_GETFL);
  if (opt < 0) {
    return -1;
  }
  return fcntl(fd, F_SETFL, opt | O_NONBLOCK);
}
