#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

#include "tidy.h"
#include "tidybuffio.h"
#include "curl/curl.h"

#include "crawl.h"

bool url_validate(char *url)
{
	regex_t regex;
	int reti;
	char msgbuf[100];
	bool match = false;

	/* Compile regular expression */
	if (regcomp(&regex, "https?:\\/\\/(www\\.)?[-a-zA-Z0-9@:%._\\+~#=]{1,256}\\.[a-zA-Z0-9()]{1,6}\\b([-a-zA-Z0-9()@:%_\\+.~#?&//=]*)", REG_EXTENDED)) {
		return false;
	}

	/* Execute regular expression */
	reti = regexec(&regex, url, 0, NULL, 0);
	if (!reti) {
		match = true;
	} else if (reti == REG_NOMATCH) {
		match = false;
	} else {
		regerror(reti, &regex, msgbuf, sizeof(msgbuf));
		fprintf(stderr, "Regex match failed: %s\n", msgbuf);
	}

	/* Free compiled regular expression if you want to use the regex_t again */
	regfree(&regex);

	return match;
}

static size_t curl_write_clb_save_to_buffer(void *contents, size_t size, size_t nmemb, void *userp)
{
	size_t realsize = size * nmemb;
	StrSlice *mem = (StrSlice *)userp;

	char *ptr = realloc(mem->ptr, mem->len + realsize + 1);
	if (!ptr) {
		/* out of memory! */
		printf("not enough memory (realloc returned NULL)\n");
		return 0;
	}

	mem->ptr = ptr;
	memcpy(&(mem->ptr[mem->len]), contents, realsize);
	mem->len += realsize;
	mem->ptr[mem->len] = '\0';

	return realsize;
}

bool url_fetch(char *url, StrSlice *buf)
{
	CURL *curl;
	CURLcode res;
	bool success = true;

	buf->ptr = malloc(1); /* grown as needed by the realloc above */
	buf->len = 0; /* no data at this point */

	curl_global_init(CURL_GLOBAL_DEFAULT);

	curl = curl_easy_init();
	if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

		/* send all data to this function  */
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_write_clb_save_to_buffer);

		/* we pass our 'chunk' struct to the callback function */
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)buf);

		/* some servers do not like requests that are made without a user-agent
		   field, so we provide one */
		curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");

		/* cache the CA cert bundle in memory for a week */
		curl_easy_setopt(curl, CURLOPT_CA_CACHE_TIMEOUT, 604800L);

		/* Perform the request, res gets the return code */
		res = curl_easy_perform(curl);
		/* Check for errors */
		if (res != CURLE_OK) {
			fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
			success = false;
		}

		/* always cleanup */
		curl_easy_cleanup(curl);
	} else {
		success = false;
	}
	curl_global_cleanup();

	return success;
}

bool html_get_uri(StrSlice content, StrArray *uris)
{
	StrSlice curent_uri;

	array_init(uris);

	/* TO-DO: .. */

	return true;
}
