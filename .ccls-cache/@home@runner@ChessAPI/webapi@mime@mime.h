#include "../../common/string.h"

/*
 | This list is still not complete and needs an automatic updater downloading a
 list from https://www.iana.org/assignments/media-types/media-types.xhtml |
 furthermorem at the moment, only file extensions work to imply a MIME type,
 but x-world/x-3dmf can be implied by file name
 |
 */

struct mime_subtype {
  str name;
  uint n_ext;
  str *ext;
};

struct mime_type {
  str name;
  uint n_subtypes;
  struct mime_subtype *subtypes;
} * mime_types;

#define MIME_TYPE_OF(name, subtypes_arr)                                       \
  (struct mime_type){STR_OF(name), sizeof(subtypes_arr), subtypes_arr};

struct mime_spec {
  ushort type;
  ushort sub;
};

struct mime_sub_types {};

uint parse_mime_type(string s); // TODO

uint is_mime_type(string s);