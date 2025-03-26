#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include <assert.h>

int main(int argc, char *argv[1]) {
	if (argc != 2) {
		fprintf(stderr, "usage: cpu <string>\n");
		exit(1);
	}
	char *str = argv[1];
	while (1) {
		sleep(1);
		printf("%s\n", str);
	}
	return 0;
}
