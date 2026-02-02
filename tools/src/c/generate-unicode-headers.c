#include <stdio.h>
#include <stdlib.h>

static void usage() {
	printf("usage: generate-unicode-headers <url>\n");
}

int main(int argc, char *argv[]) {
	if (argc != 2) {
		usage();
		exit(EXIT_FAILURE);
	}
	printf("hello %s\n", argv[1]);
	return 0;
}
