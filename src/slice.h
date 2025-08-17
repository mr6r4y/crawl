#ifndef SLICE_H
#define SLICE_H

#include <stdbool.h>
#include <stdlib.h>

typedef struct StrSlice_ {
  char *ptr;
  size_t len;
} StrSlice;

StrSlice slice_next_token(StrSlice *slice, char separator);
static inline bool slice_strcmp(StrSlice slice, const char *other);
static inline StrSlice slice_from_string(const char *data);
void slice_trim(StrSlice *slice);

#endif
