#ifndef CRAWL_H
#define CRAWL_H

#include <stddef.h>
#include <stdbool.h>

#include "slice.h"

bool url_validate(char *url);
bool url_fetch(char *url, StrSlice *buf);

#endif /* CRAWL_H */
