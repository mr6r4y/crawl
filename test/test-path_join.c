#include <stdlib.h>
#include <stdio.h>

#include "auxiliary.c"

int main()
{
	VecList *paths;
	char *joined_paths;
	veclist_init(&paths, 64, 512);
	veclist_push_str(&paths, "/aaabcd/ddd");
	veclist_push_str(&paths, "uuuu/eeee/ffff/");
	veclist_push_str(&paths, "rrrrr/xx/");
	joined_paths =path_join(paths);
	printf("Joined path: %s", joined_paths);
	free(paths);
	free(joined_paths);
}
