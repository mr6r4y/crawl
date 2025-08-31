#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "auxiliary.c"

int main()
{
#define N 100
	Vec *vec[N];
	char *s = "Hello, world !";
	int i = 0;

	for (i = 0; i < N; i++)
		vec_create(&vec[i], s, strlen(s));
	for (i = 0; i < N; i++)
		printf("%d: %s\n", i, vec[i]->data);
	for (i = 0; i < N; i++)
		free(vec[i]);
}
