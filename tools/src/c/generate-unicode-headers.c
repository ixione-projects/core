#include <curl/curl.h>
#include <curl/easy.h>
#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sysexits.h>
#include <unistd.h>

static void usage(int status) {
	printf("usage: generate-unicode-headers <url>\n");
	exit(status);
}

static size_t write_cb(char *ptr, size_t size, size_t nmemb, void *userdata) {
	return fwrite(ptr, size, nmemb, (FILE *)userdata);
}

static FILE *open_or_download_unicode_data(const char *url) {
	FILE *file = NULL;

	char *path = NULL;
	CURL *curl = NULL;

	const char *suffix = "/generate-unicode-headers/UnicodeData.txt";
#define setpath(fmt, ...)                             \
	int size = snprintf(NULL, 0, fmt, ##__VA_ARGS__); \
	if (size < 0) {                                   \
		fprintf(stderr, "snprintf: %m\n");            \
		goto fail;                                    \
	}                                                 \
	path = (char *)malloc((size_t)size + 1);          \
	sprintf(path, fmt, ##__VA_ARGS__);                \
	path[size] = '\0'

	char *cache_home = getenv("XDG_CACHE_HOME");
	if (cache_home == NULL) {
		char *home = getenv("HOME");
		if (home == NULL) {
			setpath("/tmp%s", suffix);
		} else {
			setpath("%s/.cache%s", home, suffix);
		}
	} else {
		setpath("%s%s", cache_home, suffix);
	}

	printf("-- UnicodeData.txt Path: %s\n", path);
	if ((file = fopen(path, "r")) == NULL) {
		printf("-- Not Cached - Donwloading: %s\n", url);
		if (errno != ENOENT) {
			fprintf(stderr, "open: %m\n");
			goto fail;
		}

		CURLcode result = curl_global_init(CURL_GLOBAL_ALL);
		if (result != CURLE_OK) {
			fprintf(stderr, "curl_global_init: failed\n");
			goto fail;
		}

		curl = curl_easy_init();
		if (curl == NULL) {
			fprintf(stderr, "curl_easy_init: failed\n");
			goto fail;
		}

		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_cb);

		char *p = (*path == '/' ? path + 1 : path);
		for (; *p != '\0'; ++p) {
			if (*p == '/') {
				*p = '\0';
				if (access(path, F_OK) == 0) {
					goto skip;
				}

				if (errno != ENOENT) {
					fprintf(stderr, "access: %m\n");
					goto fail;
				}

				if (mkdir(path, 0775) == -1) {
					fprintf(stderr, "mkdir: %m\n");
					goto fail;
				}

			skip:
				*p = '/';
			}
		}

		file = fopen(path, "w+");
		if (file == NULL) {
			fprintf(stderr, "open: %m\n");
			goto fail;
		}
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, file);

		result = curl_easy_perform(curl);
		if (result != CURLE_OK) {
			if (errno != 0) {
				fprintf(stderr, "curl_easy_perform: %m\n");
			} else {
				fprintf(stderr, "curl_easy_perform: failed\n");
			}
			goto fail;
		}

		curl_easy_cleanup(curl);
	}

	free(path);
	return file;

fail:
	if (file != NULL) {
		fclose(file);
	}
	free(path);
	curl_easy_cleanup(curl);
	usage(EX_SOFTWARE);
	return NULL;
#undef sprintf
}

int main(int argc, char *argv[]) {
	if (argc != 2) {
		usage(EX_USAGE);
	}

	FILE *file = open_or_download_unicode_data(argv[1]);
	char *data = NULL;

	if (fseek(file, 0, SEEK_END) == -1) {
		fprintf(stderr, "fseek: %m\n");
		goto fail;
	}
	long fsize = ftell(file);
	if (fsize == -1) {
		fprintf(stderr, "ftell: %m\n");
		goto fail;
	}
	rewind(file);

	data = (char *)malloc((size_t)fsize + 1);
	fread(data, 1, (size_t)fsize, file);
	data[fsize] = '\0';

	printf("%.*s\n", 38, data);

	fclose(file);
	free(data);
	return EXIT_SUCCESS;

fail:
	fclose(file);
	free(data);
	return EXIT_FAILURE;
}
