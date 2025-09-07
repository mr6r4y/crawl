#ifndef CRAWL_H
#define CRAWL_H

#include <stddef.h>
#include <stdbool.h>

#include "auxiliary.h"

bool url_validate(char *url);
bool url_fetch(char *url, StrSlice *buf);
int html_get_href(StrSlice content, VecList **hrefs);
void href_download(char *url, char *href, char *outdir);

#endif /* CRAWL_H */
