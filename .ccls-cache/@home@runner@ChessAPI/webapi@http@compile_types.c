#include <stdio.h>
#include <stdlib.h>

#include "../../common/string.h"

#include "compile_types.h"

int generate_header_types_c() {
  FILE *f = fopen("webapi/http/rsc/header_types.h.templ", "r");
  if (!f)
    return -1;

  fseek(f, 0, SEEK_END);

  int len = ftell(f);
  rewind(f);

  string templ = {.c = malloc(len), .len = len};

  for (int i = 0; i < templ.len; i++) {
    templ.c[i] = fgetc(f);
  }

  fclose(f);

  f = fopen("webapi/http/rsc/header_types.txt", "r");
  if (!f) {
    free(templ.c);
    return -1;
  }

  {
    char *type_name = malloc(64);
    char *type_enum = malloc(64);

    while (fgets(type_name, 64, f) != NULL) {

      memcpy(type_enum, type_name, 64);
      uint len = 0;
      for (; type_name[len] != '\n' && type_name[len] != 0; len++) {
        if (type_enum[len] == '-')
          type_enum[len] = '_';
      }
      type_name[len] = 0;
      type_enum[len] = 0;

      char *tmp0, *tmp1;

      // enum
      {
        tmp0 = malloc(len + 4);
        sprintf(tmp0, "%s,%s", type_enum, "%s");
      }

      // string
      {
        tmp1 = malloc(len + 14);
        sprintf(tmp1, "STR_OF(\"%s\"),%s", type_name, "%s");
      }

      // inserting
      {
        int new_len = templ.len + strlen(tmp0) + strlen(tmp1);
        char *tmp = malloc(new_len + 1);
        sprintf(tmp, templ.c, tmp0, tmp1);
        free(templ.c);
        templ.c = tmp;
				templ.len = new_len;
      }

      free(tmp0);
      free(tmp1);
    }

    free(type_name);
    free(type_enum);
    fclose(f);
  }

  char *tmp = malloc(templ.len);

  sprintf(tmp, templ.c, "", "");

  free(templ.c);

  templ = (string){.c = tmp, .len = templ.len - 4};

  // writing
  {

    f = fopen("webapi/http/header_types.h", "w");
    if (!f) {
      free(templ.c);
      return -1;
    }

    fprintf(f, "%s", templ.c);

    fclose(f);
  }

  free(templ.c);
  return 0;
}