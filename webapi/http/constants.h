#pragma once

typedef enum http_status {
  // 1xx Informational response
  CONTINUE,
  SWITCHING_PROTOCOLS,
  PROCESSING,
  EARLY_HINTS,

  // 2xx Success
  OK,
  CREATED,
  ACCEPTED,
  NON_AUTHORATIVE_INFORMATION,
  NO_CONTENT,
  RESET_CONTENT,
  PARTIAL_CONTENT,
  MULTI_STATUS,
  ALREADY_REPORTED,
  IM_USED,

  // 3xx Redirection
  MULTIPLE_CHOICES,
  MOVED_PERMANENTLY,
  FOUND,
  SEE_OTHER,
  NOT_MODIFIED,
  USE_PROXY,
  SWITCH_PROXY,
  TEMPORARY_REDIRECT,
  PERMANENT_REDIRECT,

  // 4xx Client Error
  BAD_REQUEST,
  UNAUTHORIZED,
  PAYMENT_REQUIRED,
  FORBIDDEN,
  NOT_FOUND,
  METHOD_NOT_ALLOWED,
  NOT_ACCEPTABLE,
  PROXY_AUTHENTICATION_REQUIRED,
  REQUEST_TIMEOUT,
  CONFLICT,
  GONE,
  LENGTH_REQUIRED,
  PRECONDITION_FAILED,
  PAYLOAD_TOO_LARGE,
  URI_TOO_LONG,
  UNSUPPORTED_MEDIA_TYPE,
  RANGE_NOT_SATISFIABLE,
  EXPECCTATION_FAILED,
  IM_A_TEAPOT,
  MISDIRECTED_REQUEST,
  UNPROCESSABLE_ENTITY,
  LOCKED,
  FAILED_DEPENDENCY,
  TOO_EARLY,
  UPGRADE_REQUIRED,
  PRECONDITION_REQUIRED,
  TOO_MANY_REQUESTS,
  REQUEST_HEADER_FIELDS_TOO_LARGE,
  UNAVAILABLE_FOR_LEGAL_REASONS,

  // 5xx Server Error
  INTERNAL_SERVER_ERROR,
  NOT_IMPLEMENTED,
  BAD_GATEWAY,
  SERVICE_UNAVAILABLE,
  GATEWAY_TIMEOUT,
  HTTP_VERSION_NOT_SUPPORTED,
  VARIANT_ALSO_NEGOTIATES,
  INSUFFICIENT_STORAGE,
  LOOP_DETECTED,
  NOT_EXTENDED,
  NETWORK_AUTHENTICATION_REQUIRED,

  NUM_HTTP_STATUS_CODES,
} HTTP_STATUS;

typedef enum http_method {
  GET,
  HEAD,
  POST,
  PUT,
  DELETE,
  CONNECT,
  OPTIONS,
  TRACE,
  NUM_HTTP_METHODS
} HTTP_METHOD;

typedef enum http_version {
  HTTP1_0,
  HTTP1_1,
  HTTP2_0,
  HTTP3_0,
	HTTP_UNKNOWN_VERSION,
  NUM_HTTP_VERSIONS,
} HTTP_VERSION;

typedef enum http_header_field_type {
  CACHE_CONTROL,
  CONNECTION,
  CONTENT_ENCODING,
  CONTENT_LENGTH,
  CONTENT_MD5,
  CONTENT_TYPE,
  DATE,
  PRAGMA,
  TRAILER,
  TRANSFER_ENCODING,
  UPGRADE,
  VIA,
  WARNING,
  NUM_UNVERSAL_HTTP_HEADERS,
} HTTP_HEADER_FIELD_TYPE;

typedef enum http_req_header_field_type {
  A_IM = NUM_UNVERSAL_HTTP_HEADERS,
  ACCEPT,
  ACCEPT_CHARSET,
  ACCEPT_DATETIME,
  ACCEPT_ENCODING,
  ACCEPT_LANGUAGE,
  ACCESS_CONTROL_REQUEST_METHOD,
  ACCESS_CONTROL_REQUEST_HEADERS,
  AUTHORIZATION,
  COOKIE,
  EXPECT,
  FORWARDED,
  FROM,
  HOST,
  HTTP2_SETTINGS,
  IF_MATCH,
  IF_MODIFIED_SINCE,
  IF_NONE_MATCH,
  IF_RANGE,
  IF_UNMODIFIED_SINCE,
  MAX_FORWARDS,
  ORIGIN,
  PREFER,
  PROXY_AUTHORIZATION,
  RANGE,
  REFERER,
  TE,
  USER_AGENT,
  NUM_HTTP_REQ_HEADERS,
} HTTP_REQ_HEADER_FIELD_TYPE;

typedef enum http_res_header_field_type {
  ACCEPT_CH = NUM_UNVERSAL_HTTP_HEADERS,
  ACCEPT_PATCH,
  ACCEPT_RANGES,
  ACCESS_CONTROL_ALLOW_CREDENTIALS,
  ACCESS_CONTROL_ALLOW_HEADERS,
  ACCESS_CONTROL_ALLOW_METHODS,
  ACCESS_CONTROL_ALLOW_ORIGIN,
  ACCESS_CONTROL_EXPOSE_HEADERS,
  ACCESS_CONTROL_MAX_AGE,
  AGE,
  ALLOW,
  ALT_SVC,
  CONTENT_DISPOSITION,
  CONTENT_LANGUAGE,
  CONTENT_LOCATION,
  CONTENT_RANGE,
  DELTA_BASE,
  ETAG,
  EXPIRES,
  IM,
  LAST_MODIFIED,
  LINK,
  LOCATION,
  P3P,
  PREFERENCE_APPLIED,
  PROXY_AUTHENTICATE,
  PUBLIC_KEY_PINS,
  RETRY_AFTER,
  SERVER,
  SET_COOKIE,
  STRICT_TRANSPORT_SECURITY,
  TK,
  VARY,
  WWW_AUTHENTICATE,
  X_FRAME_OPTIONS,
  NUM_HTTP_RES_HEADERS,
} HTTP_RES_HEADER_FIELD_TYPE;

#ifndef DEFINE_ARRAYS

extern const char *HTTP_REQ_HEADER_STRS[];
extern const char *HTTP_RES_HEADER_STRS[];
extern const char *HTTP_METHOD_STRS[];
extern const char *HTTP_STATUS_STRS[];
extern const char *HTTP_VERSION_STRS[];

#endif