#include <stdlib.h>
#include <string.h>
#include "auxiliary.h"

StrSlice slice_next_token(StrSlice *slice, char separator)
{
	size_t i;
	for (i = 0; i < slice->len; i++) {
		if (slice->ptr[i] == separator) {
			StrSlice result = { i, slice->ptr };
			slice->ptr = slice->ptr + i + 1;
			slice->len = slice->len - i - 1;
			return result;
		}
	}
	StrSlice result = *slice;
	slice->ptr = slice->ptr + slice->len;
	slice->len = 0;
	return result;
}

static inline bool slice_strcmp(StrSlice slice, const char *other)
{
	if (strlen(other) != slice.len)
		return false;
	return strncmp(slice.ptr, other, slice.len) == 0;
}

static inline StrSlice slice_from_string(char *data)
{
	StrSlice result;

	/* Include the '\0' terminating char */
	result.len = strlen(data) + 1;
	result.ptr = data;

	return result;
}

static inline void array_init(StrArray *arr)
{
	arr->len = 0;
	arr->alloc = 0;
	arr->ptr = NULL;
}

static bool array_push(StrArray *arr, StrSlice item)
{
#define ARRAY_CHUNK 16
	arr->len++;
	if (arr->len > arr->alloc) {
		arr->ptr = realloc(arr->ptr, (sizeof *arr->ptr) * (ARRAY_CHUNK + arr->alloc));
		if (!arr->ptr)
			return false;
		arr->alloc += ARRAY_CHUNK;
	}
	arr->ptr[arr->len - 1] = item;
	return true;
}

static inline StrSlice array_get(StrArray *arr, size_t ind)
{
	if (arr->len > ind)
		return arr->ptr[ind];
	else
		return (StrSlice){ 0 };
}

bool vec_create(Vec **vec, char *data, size_t data_len)
{
	Vec *v;
	*vec = malloc(sizeof(*v) + data_len + 1);

	v = *vec;
	if (!v)
		return false;

	v->len = data_len;
	v->type = DB;
	memcpy(&(v->data), data, data_len);
	v->data[data_len] = '\0';
	return true;
}
