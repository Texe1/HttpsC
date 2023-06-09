#define DEFINE_ARRAYS
#include "constants.h"

const char *HTTP_REQ_HEADER_STRS[NUM_HTTP_REQ_HEADERS] = {
    [CACHE_CONTROL] = "Cache-Control",
    [CONNECTION] = "Connection",
    [CONTENT_ENCODING] = "Content-Encoding",
    [CONTENT_LENGTH] = "Content-Length",
    [CONTENT_MD5] = "Content-MD5",
    [CONTENT_TYPE] = "Content-Type",
    [DATE] = "Date",
    [PRAGMA] = "Pragma",
    [TRAILER] = "Trailer",
    [TRANSFER_ENCODING] = "Transfer-Encoding",
    [UPGRADE] = "Upgrade",
    [VIA] = "Via",
    [WARNING] = "Warning",

    [A_IM] = "A-IM",
    [ACCEPT] = "Accept",
    [ACCEPT_CHARSET] = "Accept-Charset",
    [ACCEPT_DATETIME] = "Accept-Datetime",
    [ACCEPT_ENCODING] = "Accept-Encoding",
    [ACCEPT_LANGUAGE] = "Accept-Language",
    [ACCESS_CONTROL_REQUEST_METHOD] = "Access-Control-Request-Method",
    [ACCESS_CONTROL_REQUEST_HEADERS] = "Access-Control-Request-Headers",
    [AUTHORIZATION] = "Authorization",
    [COOKIE] = "Cookie",
    [EXPECT] = "Expect",
    [FORWARDED] = "Forwarded",
    [FROM] = "From",
    [HOST] = "Host",
    [HTTP2_SETTINGS] = "HTTP2-Settings",
    [IF_MATCH] = "If-Match",
    [IF_MODIFIED_SINCE] = "If-Modified-Since",
    [IF_NONE_MATCH] = "If-None-Match",
    [IF_RANGE] = "If-Range",
    [IF_UNMODIFIED_SINCE] = "If-Unmodified-Since",
    [MAX_FORWARDS] = "Max-Forwards",
    [ORIGIN] = "Origin",
    [PREFER] = "Prefer",
    [PROXY_AUTHORIZATION] = "Proxy-Autorization",
    [RANGE] = "Range",
    [REFERER] = "Referer",
    [TE] = "TE",
    [USER_AGENT] = "User-Agent",
};

const char *HTTP_RES_HEADER_STRS[NUM_HTTP_RES_HEADERS] = {
    [CACHE_CONTROL] = "Cache-Control",
    [CONNECTION] = "Connection",
    [CONTENT_ENCODING] = "Content-Encoding",
    [CONTENT_LENGTH] = "Content-Length",
    [CONTENT_MD5] = "Content-MD5",
    [CONTENT_TYPE] = "Content-Type",
    [DATE] = "Date",
    [PRAGMA] = "Pragma",
    [TRAILER] = "Trailer",
    [TRANSFER_ENCODING] = "Transfer-Encoding",
    [UPGRADE] = "Upgrade",
    [VIA] = "Via",
    [WARNING] = "Warning",

    [ACCEPT_CH] = "Accept-CH",
    [ACCESS_CONTROL_ALLOW_ORIGIN] = "Access-Control-Allow-Origin",
    [ACCESS_CONTROL_ALLOW_CREDENTIALS] = "Access-Control-Allow-Credentials",
    [ACCESS_CONTROL_EXPOSE_HEADERS] = "Access-Control-Expose-Headers",
    [ACCESS_CONTROL_MAX_AGE] = "Access-Control-Max-Age",
    [ACCESS_CONTROL_ALLOW_METHODS] = "Access-Control-Allow-Methods",
    [ACCESS_CONTROL_ALLOW_HEADERS] = "Access-Control-Allow-Headers",
    [ACCEPT_PATCH] = "Accept-Patch",
    [ACCEPT_RANGES] = "Accept-Ranges",
    [AGE] = "Age",
    [ALLOW] = "Allow",
    [ALT_SVC] = "Alt-Svc",
    [CONTENT_DISPOSITION] = "Content-Disposition",
    [CONTENT_LANGUAGE] = "Content-Language",
    [CONTENT_LOCATION] = "Content-Location",
    [CONTENT_RANGE] = "Content-Range",
    [DELTA_BASE] = "Delta-Base",
    [ETAG] = "ETag",
    [EXPIRES] = "Expires",
    [IM] = "IM",
    [LAST_MODIFIED] = "Last-Modified",
    [LINK] = "Link",
    [LOCATION] = "Location",
    [P3P] = "P3P",
    [PREFERENCE_APPLIED] = "Preference-Applied",
    [PROXY_AUTHENTICATE] = "Proxy-Authenticate",
    [PUBLIC_KEY_PINS] = "Public-Key-Pins",
    [RETRY_AFTER] = "Retry-After",
    [SERVER] = "Server",
    [SET_COOKIE] = "Set-Cookie",
    [STRICT_TRANSPORT_SECURITY] = "Strict-Transport-Security",
    [TK] = "TK",
    [VARY] = "Vary",
    [WWW_AUTHENTICATE] = "WWW-Authenticate",
    [X_FRAME_OPTIONS] = "X-Frame-Options",
};

const char *HTTP_METHOD_STRS[NUM_HTTP_METHODS] = {
    [GET] = "GET",         [HEAD] = "HEAD",       [POST] = "POST",
    [PUT] = "PUT",         [OPTIONS] = "OPTIONS", [TRACE] = "TRACE",
    [CONNECT] = "CONNECT", [DELETE] = "DELETE",
};

const char *HTTP_STATUS_STRS[NUM_HTTP_STATUS_CODES] = {
    [CONTINUE] = "100 Continue",
    [SWITCHING_PROTOCOLS] = "101 Swichting Protocols",
    [PROCESSING] = "102 Processing",
    [EARLY_HINTS] = "103 Early Hints",

    [OK] = "200 OK",
    [CREATED] = "201 Created",
    [ACCEPTED] = "202 Accepted",
    [NON_AUTHORATIVE_INFORMATION] = "203 Non-Authoritative Information",
    [NO_CONTENT] = "204 No Content",
    [RESET_CONTENT] = "205 Reset Content",
    [PARTIAL_CONTENT] = "206 Partial Content",
    [MULTI_STATUS] = "207 Multi-Status",
    [ALREADY_REPORTED] = "208 Already Reported",
    [IM_USED] = "226 IM Used",

    [MULTIPLE_CHOICES] = "300 Multiple Choices",
    [MOVED_PERMANENTLY] = "301 Moved Permanently",
    [FOUND] = "302 Found",
    [SEE_OTHER] = "303 See Other",
    [NOT_MODIFIED] = "304 Not Modified",
    [USE_PROXY] = "305 Use Proxy",
    [SWITCH_PROXY] = "306 Switch Proxy",
    [TEMPORARY_REDIRECT] = "307 Temporary Redirect",
    [PERMANENT_REDIRECT] = "308 Permanent Redirect",

    [BAD_REQUEST] = "400 Bad Request",
    [UNAUTHORIZED] = "401 Unauthorized",
    [PAYMENT_REQUIRED] = "402 Payment Required",
    [FORBIDDEN] = "403 Forbidden",
    [NOT_FOUND] = "404 Not Found",
    [METHOD_NOT_ALLOWED] = "405 Method Not Allowed",
    [NOT_ACCEPTABLE] = "406 Not Acceptable",
    [PROXY_AUTHENTICATION_REQUIRED] = "407 Proxy Authentication Required",
    [REQUEST_TIMEOUT] = "408 Request Timeout",
    [CONFLICT] = "409 Conflict",
    [GONE] = "410 Gone",
    [LENGTH_REQUIRED] = "411 Length Required",
    [PRECONDITION_FAILED] = "412 Precondition Failed",
    [PAYLOAD_TOO_LARGE] = "413 Payload Too Large",
    [URI_TOO_LONG] = "414 URI Too Long",
    [UNSUPPORTED_MEDIA_TYPE] = "415 Unsupported Media Type",
    [RANGE_NOT_SATISFIABLE] = "416 Range Not Satisfiable",
    [EXPECCTATION_FAILED] = "417 Expectation Failed",
    [IM_A_TEAPOT] = "418 I'm a teapot",
    [MISDIRECTED_REQUEST] = "421 Misdirected Request",
    [UNPROCESSABLE_ENTITY] = "422 Unprocessable Entity",
    [LOCKED] = "423 Locked",
    [FAILED_DEPENDENCY] = "424 Failed Dependency",
    [TOO_EARLY] = "425 Too Early",
    [UPGRADE_REQUIRED] = "426 Upgrade Required",
    [PRECONDITION_REQUIRED] = "428 Precondition Required",
    [TOO_MANY_REQUESTS] = "429 Too Many Requests",
    [REQUEST_HEADER_FIELDS_TOO_LARGE] = "431 Request Header Fields Too Large",
    [UNAVAILABLE_FOR_LEGAL_REASONS] = "451 Unavailable For Legal Reasons",

    [INTERNAL_SERVER_ERROR] = "500 Internal Server Error",
    [NOT_IMPLEMENTED] = "501 Not Implemented",
    [BAD_GATEWAY] = "502 Bad Gateway",
    [SERVICE_UNAVAILABLE] = "503 Service Unavailable",
    [GATEWAY_TIMEOUT] = "504 Gateway Timeout",
    [HTTP_VERSION_NOT_SUPPORTED] = "505 HTTP Version Not Supported",
    [VARIANT_ALSO_NEGOTIATES] = "506 Variant Also Negotiates",
    [INSUFFICIENT_STORAGE] = "507 Insufficient Storage",
    [LOOP_DETECTED] = "508 Loop Detected",
    [NOT_EXTENDED] = "510 Not Extended",
    [NETWORK_AUTHENTICATION_REQUIRED] = "511 Network Authentication Required",
};

const char *HTTP_VERSION_STRS[NUM_HTTP_VERSIONS] = {
    "1.0", "1.1", "2", "3", "x.x",
};