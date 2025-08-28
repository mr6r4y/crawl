#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "auxiliary.c"

int main()
{
	StrSlice s;
	StrArray arr;
	int i = 0;

	array_init(&arr);

	for (i = 0; i < 400; i++) {
		s = slice_from_string("Hello");
		array_push(&arr, s);

		s = slice_from_string("world");
		array_push(&arr, s);

		s = slice_from_string("!");
		array_push(&arr, s);
	}

	for (i = 0; i < arr.len; i++)
		printf("%d: %s\n", i, array_get(&arr, i).ptr);
}
