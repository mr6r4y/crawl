#include <stdint.h>
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

	if (data)
		memcpy(&(v->data), data, data_len);
	v->data[data_len] = '\0';
	return true;
}

static inline size_t vec_size(Vec *vec)
{
	return (uintptr_t)&vec->data + vec->len + 1 - (uintptr_t)vec;
}

static inline bool veclist_init(VecList **vlist, size_t init_alloc, size_t realloc_step)
{
	VecList *vl;
	size_t alloc, min_size;

	min_size = sizeof(*vl);
	alloc = init_alloc > min_size ? init_alloc : min_size;
	*vlist = malloc(alloc);
	vl = *vlist;

	if (!vl)
		return false;

	vl->len = 0;
	vl->alloc = alloc;
	vl->realloc_step = realloc_step;
	vl->end = (uintptr_t)&vl[1] - (uintptr_t)vl;

	return true;
}

static inline size_t veclist_free_size(VecList *vlist)
{
	return vlist->alloc - vlist->end;
}

static bool veclist_push_str(VecList **vec_list, char *str)
{
	VecList *vl;
	Vec *v;
	size_t free, vsize, str_len;

	vl = *vec_list;
	free = veclist_free_size(vl);
	str_len = strlen(str);
	vsize = sizeof(Vec) + str_len + 1;

	if (free < vsize) {
		vl = realloc(vl, vl->alloc + vsize - free + vl->realloc_step);
		if (!vl)
			return false;
		*vec_list = vl;
		vl->alloc += vsize - free + 16;
	}
	v = (Vec *)((uintptr_t)vl + vl->end);
	v->len = str_len;
	memcpy(&v->data, str, v->len);

	v->data[v->len] = '\0';
	vl->end += vec_size(v);
	vl->len += 1;

	return true;
}

static bool veclist_push_zero(VecList **vec_list, size_t size)
{
	VecList *vl;
	Vec *v;
	size_t free, vsize;

	vl = *vec_list;
	free = veclist_free_size(vl);
	vsize = sizeof(Vec) + size + 1;

	if (free < vsize) {
		vl = realloc(vl, vl->alloc + vsize - free + vl->realloc_step);
		if (!vl)
			return false;
		*vec_list = vl;
		vl->alloc += vsize - free + 16;
	}
	v = (Vec *)((uintptr_t)vl + vl->end);
	v->len = size;
	v->data[v->len] = '\0';
	vl->end += vec_size(v);
	vl->len += 1;

	return true;
}

static bool veclist_push(VecList **vec_list, Vec *v)
{
	VecList *vl;
	size_t free, vsize;

	vl = *vec_list;
	free = veclist_free_size(vl);
	vsize = vec_size(v);

	if (free < vsize) {
		vl = realloc(vl, vl->alloc + vsize - free + vl->realloc_step);
		if (!vl)
			return false;
		*vec_list = vl;
		vl->alloc += vsize - free + 16;
	}
	memcpy((VecList *)((uintptr_t)vl + vl->end), v, vsize);
	vl->end += vsize;
	vl->len += 1;

	return true;
}

static Vec *veclist_get(VecList *vlist, size_t ind)
{
	Vec *v;
	size_t i = 0;
	size_t vsz;

	if (ind >= vlist->len)
		return NULL;

	v = (Vec *)&vlist[1];
	while (ind != i) {
		vsz = vec_size(v);
		v = (Vec *)((uintptr_t)v + vsz);

		i++;
	}

	return v;
}

#ifndef PATH_SEP
# define PATH_SEP "/"
#endif

static char *veclist_path_join(VecList *paths)
{
	char *s;
	Vec *v;
	size_t i, sl;

	sl = paths->end + strlen(PATH_SEP) * (paths->len - 1) - (paths->len * sizeof(Vec)) + 1;
	s = malloc(sl);
	s[sl - 1] = '\0';

	for (i = 0; i < paths->len - 1; i++) {
		v = veclist_get(paths, i);
		strcat(s, v->data);
		strcat(s, PATH_SEP);
	}
	v = veclist_get(paths, i);
	strcat(s, v->data);

	return s;
}
