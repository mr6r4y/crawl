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
	VecList *vl;

	veclist_init(&vl, 128);

	for (i = 0; i < N; i++)
		vec_create(&vec[i], s, strlen(s));
	for (i = 0; i < N; i++)
		veclist_push(&vl, vec[i]);
	for (i = 0; i < N; i++)
		free(vec[i]);

	for (i = 0; i < vl->len; i++)
		printf("%d: %s\n", i, veclist_get(vl, i)->data);

	free(vl);
}

