#pragma once

#include <stdlib.h>

#include "../../common/string.h" 

enum HTTP_HEADER_TYPE {
A_IM,Accept,Accept_Charset,Accept_Datetime,Accept_Encoding,Accept_Language,Access_Control_Request_Method,Access_Control_Request_Headers,Authorization,Cache_Control,Connection,Content_Encoding,Content_Length,Content_MD5,Content_Type,Cookie,Date,Expect,Forwarded,From,Host,HTTP2_Settings,If_Match,If_Modified_Since,If_None_Match,If_Range,If_Unmodified_Since,Max_Forwards,Origin,Pragma,Prefer,Proxy_Authorization,Range,Referer,TE,Trailer,Transfer_Encoding,User_Agent,Upgrade,Via,Warning,};

static string HTTP_HEADER_TYPE_STRINGS[] = {
STR_OF("A-IM"),STR_OF("Accept"),STR_OF("Accept-Charset"),STR_OF("Accept-Datetime"),STR_OF("Accept-Encoding"),STR_OF("Accept-Language"),STR_OF("Access-Control-Request-Method"),STR_OF("Access-Control-Request-Headers"),STR_OF("Authorization"),STR_OF("Cache-Control"),STR_OF("Connection"),STR_OF("Content-Encoding"),STR_OF("Content-Length"),STR_OF("Content-MD5"),STR_OF("Content-Type"),STR_OF("Cookie"),STR_OF("Date"),STR_OF("Expect"),STR_OF("Forwarded"),STR_OF("From"),STR_OF("Host"),STR_OF("HTTP2-Settings"),STR_OF("If-Match"),STR_OF("If-Modified-Since"),STR_OF("If-None-Match"),STR_OF("If-Range"),STR_OF("If-Unmodified-Since"),STR_OF("Max-Forwards"),STR_OF("Origin"),STR_OF("Pragma"),STR_OF("Prefer"),STR_OF("Proxy-Authorization"),STR_OF("Range"),STR_OF("Referer"),STR_OF("TE"),STR_OF("Trailer"),STR_OF("Transfer-Encoding"),STR_OF("User-Agent"),STR_OF("Upgrade"),STR_OF("Via"),STR_OF("Warning"),};

#define N_HEADER_TYPES (sizeof(HTTP_HEADER_TYPE_STRINGS) / sizeof(string))

uint get_http_header_type_idx(string s);