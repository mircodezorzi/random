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

	settings->has_path_input  = 0;
	settings->has_path_output = 0;

	for (int i = 1; i < argc; i++) {
		switch (argv[i][0]) {
		case '-':
			switch (argv[i][1]) {
			case 'm': arg_memory:
				settings->size = scan_number(argv[i + 1]);
				break;
			case 'i': arg_path_input:
				scan_path(argv[i + 1], settings->path_input, 512);
				settings->has_path_input = 1;
				break;
			case 'o': arg_path_output:
				scan_path(argv[i + 1], settings->path_output, 512);
				settings->has_path_output = 1;
				break;
			case 't': arg_transpiler:
				settings->interpreter = Transpiler;
				break;
			case '-':
				if (strcmp("memory", argv[i] + 2) == 0)
					goto arg_memory;
				else if (strcmp("input", argv[i] + 2) == 0)
					goto arg_path_input;
				else if (strcmp("output", argv[i] + 2) == 0)
					goto arg_path_output;
				else if (strcmp("transpiler", argv[i] + 2) == 0)
					goto arg_transpiler;
				break;
			default:
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
	fprintf(stderr, "  -m, --memory  set tape memory\n");
	fprintf(stderr, "  -i, --input   input path for code\n");
	fprintf(stderr, "  -o, --output  output path for transpiled code\n");
	_exit(1);
}
