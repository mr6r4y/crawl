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

static inline bool veclist_init(VecList **vlist, size_t init_alloc)
{
	VecList *vl;
	size_t alloc;

	alloc = init_alloc > sizeof(*vl) ? init_alloc : sizeof(*vl);
	*vlist = malloc(alloc);
	vl = *vlist;

	if (!vl)
		return false;

	vl->len = 0;
	vl->alloc = alloc;

	return true;
}

static bool veclist_push(VecList **vec_list, Vec *v)
{
	Vec *current;
	VecList *vlist;
	size_t vlen;
	size_t vsize = 0;
	size_t i = 0;

	vlist = *vec_list;
	current = &vlist->body[0];
	vsize += sizeof(*vlist);
	vsize += (size_t)(&current->data[current->len]) - (size_t)current;
	while (i < vlist->len) {
		current = (Vec *)(&current->data[current->len + 1]);
		vsize += (size_t)(&current->data[current->len]) - (size_t)current;
		i++;
	}

	vlen = (size_t)(&v->data[v->len]) - (size_t)v;
	vsize += vlen;

	if (vlist->alloc < vsize) {
		vlist->alloc = vsize + 8 - vsize % 8;
		vlist = realloc(vlist, vlist->alloc);
		if (!vlist)
			return false;
	}

	memcpy(current, v, vlen);
	vlist->len += 1;

	*vec_list = vlist;

	return true;
}

static Vec *veclist_get(VecList *vlist, size_t ind)
{
	Vec *v;
	size_t i = 0;

	if (ind >= vlist->len)
		return NULL;

	v = &vlist->body[0];
	while (ind != i) {
		if ((size_t)&v->data + v->len + 1 < (size_t)&v->data)
			return NULL;
		v = (Vec *)((size_t)&v->data + v->len + 1);

		i++;
	}

	return v;
}
