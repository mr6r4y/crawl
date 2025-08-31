#ifndef SLICE_H
#define SLICE_H

#include <stdbool.h>
#include <stddef.h>

typedef struct StrSlice_ {
  size_t len;
  char *ptr;
} StrSlice;

typedef struct StrArray_ {
  size_t len;
  size_t alloc;
  StrSlice *ptr;
} StrArray;

typedef enum VecType_ {
  DB,
  WORD,
  DWORD,
  QWORD,
} VecType;

typedef struct Vec_ {
  size_t len;
  VecType type;
  char data[1];
} Vec;

typedef struct VecList_ {
  size_t len;
  size_t alloc;
  Vec body[1];
} VecList;

StrSlice slice_next_token(StrSlice *slice, char separator);
static inline bool slice_strcmp(StrSlice slice, const char *other);
static inline StrSlice slice_from_string(char *data);
void slice_trim(StrSlice *slice);

static inline void array_init(StrArray *arr);
static bool array_push(StrArray *arr, StrSlice item);
static inline StrSlice array_get(StrArray *arr, size_t ind);

/* Creates a Vec by malloc. Copies data to vec->data.
 * When destroyed use free().
*/
bool vec_create(Vec **vec, char *data, size_t data_len);

#endif
