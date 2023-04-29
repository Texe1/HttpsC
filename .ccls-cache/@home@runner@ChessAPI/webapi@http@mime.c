#include "mime.h"

uint parse_mime_type(string s) { // TODO make lookup
}

uint mime_type_matches(string s, struct mime_type type);
uint is_mime_type(string s, struct mime_type type){
	if(type.type > X_WORLD || type.sub >= MIME_NUM_SUBTYPES[type.type]){
		return -1;
	}

	
}