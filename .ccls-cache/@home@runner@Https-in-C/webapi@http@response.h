#pragma once

#include <stdlib.h>

#include "HTTP_CONSTANTS.h"

typedef struct http_response {
	HTTP_VERSION version;
	HTTP_STATUS status;
	
} HTTP_RESPONSE;