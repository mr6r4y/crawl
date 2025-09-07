#ifndef CRAWL_H
#define CRAWL_H

#include <stddef.h>
#include <stdbool.h>

#include "auxiliary.h"

bool url_validate(char *url);
bool url_fetch(char *url, StrSlice *buf);
int html_get_uri(StrSlice content, VecList **hrefs);

#endif /* CRAWL_H */
