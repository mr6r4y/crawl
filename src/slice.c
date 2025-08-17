#include <string.h>
#include "slice.h"

StrSlice slice_next_token(StrSlice *slice, char separator)
{
	size_t i;
	for (i = 0; i < slice->len; i++) {
		if (slice->ptr[i] == separator) {
			StrSlice result = { slice->ptr, i };
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

static inline StrSlice slice_from_string(const char *data)
{
	return (StrSlice){ data, strlen(data) };
}
