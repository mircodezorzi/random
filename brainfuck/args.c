#include "args.h"

#include <stdio.h>
#include <unistd.h>
#include <string.h>

static inline long
scan_number(char *buffer)
{
	long n = 0;
	for (int i = 0; buffer[i] >= '0' && buffer[i] <= '9'; i++)
		n = n * 10 + (buffer[i] - '0');
	return n;
}

static inline void
scan_path(char *in, char* out, int n)
{
	memset(out, 0, n);
	for (int i = 0; in[i] != '\0' && i < n; i++)
		out[i] = in[i];
}

void
handle(int argc, char *argv[], struct settings *settings)
{
	if (argc < 2) usage();
	for (int i = 1; i < argc; i++) {
		switch (argv[i][0]) {
		case '-':
			switch (argv[i][1]) {
			case 'm': arg_memory:
				settings->size = scan_number(argv[i + 1]);
				break;
			case 'i': arg_path_input:
				scan_path(argv[i + 1], settings->path_input, 512);
				break;
			case 'o': arg_path_output:
				scan_path(argv[i + 1], settings->path_output, 512);
				break;
			case '-':
				break;
			}
			break;
		}
	}
}

void
usage()
{
	fprintf(stderr, "Usage:  bc [OPTIONS] code\n\n");
	fprintf(stderr, "  -i Interpterer\n");
	fprintf(stderr, "  -t Transpiler\n");
	fprintf(stderr, "  -p Pretty Transpiler (may run in constexpr mode)\n");
	_exit(1);
}
