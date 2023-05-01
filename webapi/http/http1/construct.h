#pragma once

#include "../request.h"
#include "../response.h"

char *http1_construct_request(HTTP_REQUEST *);
char *http1_construct_response(HTTP_RESPONSE *);