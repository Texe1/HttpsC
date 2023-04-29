#include "status_codes.h"

#include <stdio.h>
#include <string.h>

struct status_code {
  uint code;
  const char *str;
} status_codes[] = {

    {100, "Continue"},
    {101, "Switching Protocols"},
    {102, "Processing"},
    {103, "Early Hints"},

    {200, "OK"},
    {201, "Created"},
    {202, "Accepted"},
    {203, "Non-Authoritative Information"},
    {204, "No Content"},
    {205, "Reset Content"},
    {206, "Partial Content"},
    {207, "Multi-Status"},
    {208, "Already Reported"},
    {226, "IM Used"},

    {300, "Multiple Choices"},
    {301, "Moved Permanently"},
    {302, "Found"},
    {303, "See Other"},
    {304, "Not Modified"},
    {305, "Use Proxy"},
    {306, "Switch Proxy"},
    {307, "Temporary Redirect"},
    {308, "Permanent Redirect"},

    {400, "Bad Request"},
    {401, "Unauthorized"},
    {402, "Payment Required"},
    {403, "Forbidden"},
    {404, "Not Found"},
    {405, "Method Not Allowed"},
    {406, "Not Acceptable"},
    {407, "Proxy Authentication Required"},
    {408, "Request Timeout"},
    {409, "Conflict"},
    {410, "Gone"},
    {411, "Length Required"},
    {412, "Preconition Failed"},
    {413, "Payload Too Large"},
    {414, "URI Too Long"},
    {415, "Unsupported Media Type"},
    {416, "Range Not Satisfiable"},
    {417, "Expectation Failed"},
    {418, "I'm a teapot"},
    {421, "Misdirected Request"},
    {422, "Unprocessable Entity"},
    {423, "Locked"},
    {424, "Failed Dependency"},
    {425, "Too Early"},
    {426, "Upgrade Required"},
    {428, "Precondition Required"},
    {429, "Too Many Requests"},
    {431, "Request Header Fields Too Large"},
    {432, "Unavailable For Legal Reasons"},

    {500, "Internal Server Error"},
    {501, "Not Implemented"},
    {502, "Bad Gateway"},
    {503, "Service Unavailable"},
    {504, "Gateway Timeout"},
    {505, "HTTP Version Not Supported"},
    {506, "Variant Also Negotiates"},
    {507, "Insufficient Storage"},
    {508, "Loop Detected"},
    {510, "Not Extended"},
    {511, "Network Authentication Required"}

};

struct string http_code_search(uint code, uint lower, uint upper);

struct string get_http_code_desc(uint code) {
  return http_code_search(code, 0, sizeof(status_codes) / sizeof(struct status_code));
}

struct string http_code_search(uint code, uint lower, uint upper) {
  if (lower == upper) {
    if (status_codes[lower].code == code) {
      return (struct string){.c = (char *)status_codes[lower].str,
                             .len = strlen(status_codes[lower].str),
                             .null_terminated = 1,
                             .EOF_terminated = 0,
                             .allocated = 0};
    }
    return (struct string){0};
  }
  int avg = (lower + upper) / 2;

  if (status_codes[avg].code > code) { // search on the left
    return http_code_search(code, lower, avg - 1);
  }
  if (status_codes[avg].code < code) { // search on the right
    return http_code_search(code, avg + 1, upper);
  }

  // spot on
  return (struct string){.c = (char *)status_codes[avg].str,
                         .len = strlen(status_codes[avg].str),
                         .null_terminated = 1,
                         .EOF_terminated = 0,
                         .allocated = 0};
}