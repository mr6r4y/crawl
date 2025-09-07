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

typedef struct Vec_ Vec;

typedef struct VecExtra_ {
  Vec *next;
  Vec *prev;
} VecExtra;

struct Vec_ {
  size_t len;
  VecExtra extra;
  char data[1];
};

typedef struct VecList_ VecList;
struct VecList_ {
  size_t len;
  size_t alloc;
  size_t end;
};

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

/* Initializes vlist by malloc
 */
static inline bool veclist_init(VecList **vlist, size_t init_alloc);
static bool veclist_push(VecList **vlist, Vec *v);
static bool veclist_push_str(VecList **vec_list, char *str);
static bool veclist_push_zero(VecList **vec_list, size_t size);
static Vec *veclist_get(VecList *vlist, size_t ind);

#endif
