#include "http.h"

char* default_http_Response(char* req, void* data){
	return "HTTP/1.1 403 Forbidden\r\n\r\n";
}