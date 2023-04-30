#include "response.h"

#include "response.h"

#include <stdio.h>
#include <string.h>

#include "http_constants.h"

extern const char *HTTP_VERSION_STRS[];

const char *STATUS_CODE_STRS[] = {"100 Continue",
                                  "101 Swichting Protocols",
                                  "102 Processing",
                                  "103 Early Hints",

                                  "200 OK",
                                  "201 Created",
                                  "202 Accepted",
                                  "203 Non-Authoritative Information",
                                  "204 No Content",
                                  "205 Reset Content",
                                  "206 Partial Content",
                                  "207 Multi-Status",
                                  "208 Already Reported",
                                  "226 IM Used",

                                  "300 Multiple Choices",
                                  "301 Moved Permanently",
                                  "302 Found",
                                  "303 See Other",
                                  "304 Not Modified",
                                  "305 Use Proxy",
                                  "306 Switch Proxy",
                                  "307 Temporary Redirect",
                                  "308 Permanent Redirect",

                                  "400 Bad Request",
                                  "401 Unauthorized",
                                  "402 Payment Required",
                                  "403 Forbidden",
                                  "404 Not Found",
                                  "405 Method Not Allowed",
                                  "406 Not Acceptable",
                                  "407 Proxy Authentication Required",
                                  "408 Request Timeout",
                                  "409 Conflict",
                                  "410 Gone",
                                  "411 Length Required",
                                  "412 Precondition Failed",
                                  "413 Payload Too Large",
                                  "414 URI Too Long",
                                  "415 Unsupported Media Type",
                                  "416 Range Not Satisfiable",
                                  "417 Expectation Failed",
                                  "418 I'm a teapot",
                                  "421 Misdirected Request",
                                  "422 Unprocessable Entity",
                                  "423 Locked",
                                  "424 Failed Dependency",
                                  "425 Too Early",
                                  "426 Upgrade Required",
                                  "428 Precondition Required",
                                  "429 Too Many Requests",
                                  "431 Request Header Fields Too Large",
                                  "451 unavailable For Legal Reasons",

                                  "500 Internal Server Error",
                                  "501 Not Implemented",
                                  "502 Bad Gateway",
                                  "503 Service Unavailable",
                                  "504 Gateway Timeout",
                                  "505 HTTP Version Not Supported",
                                  "506 Variant Also Negotiates",
                                  "507 Insufficient Storage",
                                  "508 Loop Detected",
                                  "510 Not Extended",
                                  "511 Network Authentication Required"};

const char *HTTP_RES_HEADER_FIELD_STRS[NUM_RES_HEADERS] = {
    "Cache-Control",
    "Connection",
    "Content-Encoding",
    "Content-Length",
    "Content-MD5",
    "Content-Type",
    "Date",
    "Pragma",
    "Trailer",
    "Transfer-Encoding",
    "Upgrade",
    "Via",
    "Warning",
    "Accept-CH",
    "Access-Control-Allow-Origin",
    "Access-Control-Allow-Credentials",
    "Access-Control-Expose-Headers",
    "Access-Control-Max-Age",
    "Access-Control-Allow-Methods",
    "Access-Control-Allow-Headers",
    "Accept-Patch",
    "Accept-Ranges",
    "Age",
    "Allow",
    "Alt-Svc",
    "Content-Disposition",
    "Content-Language",
    "Content-Location",
    "Content-Range",
    "Delta-Base",
    "ETag",
    "Expires",
    "IM",
    "Last-Modified",
    "Link",
    "Location",
    "P3P",
    "Preference-Applied",
    "Proxy-Authenticate",
    "Public-Key-Pins",
    "Retry-After",
    "Server",
    "Set-Cookie",
    "Strict-Transport-Security",
    "TK",
    "Vary",
    "WWW-Authenticate",
    "X-Frame-Options",
};

char *formulateResponse(HTTP_RESPONSE *res) {

  // Getting the final length
  uint len = 0;
  {
    len = 5                                         // HTTP/
          + strlen(HTTP_VERSION_STRS[res->version]) // Version
          + 1                                       // SP
          + strlen(STATUS_CODE_STRS[res->status])   // Status
          + 2;                                      // CRLF

    for (uint i = 0; i < NUM_RES_HEADERS; i++) {
      if (res->headers[i]) {
        len += strlen(HTTP_RES_HEADER_FIELD_STRS[i]) // header name
               + 1                                   // :
               + strlen(res->headers[i])             // header value
               + 2;                                  // CRLF
      }
    }

    len += 2                  // CRLF
           + res->max_body_sz // body
           + 1;               // NULL
  }

  uint i = 0;
  char *s = malloc(len + 1);

  if (!s) {
    perror("Out of Heap.\n");
    return NULL;
  }

  sprintf(s, "HTTP/%s %s\r\n", HTTP_VERSION_STRS[res->version],
          STATUS_CODE_STRS[res->status]);

  i += 8 + strlen(HTTP_VERSION_STRS[res->version]) +
       strlen(STATUS_CODE_STRS[res->status]);

  for (uint j = 0; j < NUM_RES_HEADERS; j++) {
    if (res->headers[j]) {
      const char *name = HTTP_RES_HEADER_FIELD_STRS[j];
      const char *val = res->headers[j];
      uint n = strlen(name);
      memcpy(s + i, name, n);
      i += n;
      s[i++] = ':';
      n = strlen(val);
      memcpy(s + i, val, n);
      i += n;
      s[i++] = '\r';
      s[i++] = '\n';
    }
  }

  s[i++] = '\n';
  s[i++] = '\r';

  memcpy(s + i, res->body, res->max_body_sz);
  i += res->max_body_sz;

  s[i] = 0;
  return s;
}