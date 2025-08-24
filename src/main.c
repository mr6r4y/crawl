#include <stddef.h>
#include <argp.h>
#include <time.h>

/*
 * Unity build sources
 * */
#include "slice.c"
#include "crawl.c"
/*
 * -------------------
 */

typedef struct Parameters_ {
	int version;
	char *output_dir;
	char *url;
} Parameters;

static error_t parse_opt(int key, char *arg, struct argp_state *state)
{
	/* Get the input argument from argp_parse, which we
	know is a pointer to our arguments structure. */
	Parameters *arguments = state->input;
	switch (key) {
	case 'v':
		arguments->version = 1;
		break;
	case 'o':
		arguments->output_dir = arg;
		break;
	case ARGP_KEY_ARG:
		if (state->arg_num >= 1)
			/* Too many arguments. */
			argp_usage(state);
		arguments->url = arg;
		break;
	case ARGP_KEY_END:
		if (state->arg_num < 1)
			/* Not enough arguments. */
			argp_usage(state);
		break;
	default:
		return ARGP_ERR_UNKNOWN;
	}
	return 0;
}

int main(int argc, char **argv)
{
	StrSlice fetch_buf;
	Parameters params;
	StrArray uris;

	/* Default arguments */
	params.version = 0;
	params.output_dir = "out";
	params.url = "";
	/* ----------------- */

	int remaining = 0;
	int i = 0;

	struct argp_option options[] = {
		{ "version", 'v', NULL, 0, "Print version string" },
		{ "output_dir", 'o', "OUTPUT_DIR", 0, "Set the output directory where URI resources are downloaded" },
		{ 0 }
	};
	char *doc = "Crawl for content in default Apache directory listing";
	char *args_doc = "URL";
	struct argp argp = {
		options,
		parse_opt,
		args_doc,
		doc
	};
	argp_parse(&argp, argc, argv, 0, 0, &params);
	printf("URL: %s\n", params.url);
	printf("Output-Dir: %s\n", params.output_dir);

	if (url_validate(params.url))
		if (url_fetch(params.url, &fetch_buf)) {
			// printf("CONTENT:\n===================\n\n%s\n\n=========================\n", fetch_buf.ptr);
			html_get_uri(fetch_buf, &uris);
			for (i = 0; i < uris.len; i++)
				printf("%d: %s\n", i, uris.ptr[i].ptr);
		} else
			printf("Error: Cannot fetch URL: %s", params.url);
	else
		printf("Error: Invalid URL: %s", params.url);

	return EXIT_SUCCESS;
}
