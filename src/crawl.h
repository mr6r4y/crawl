#include <stddef.h>

typedef struct Buffer_ {
  char *buf;
  size_t size;
} Buffer;

int url_validate(char *url);
