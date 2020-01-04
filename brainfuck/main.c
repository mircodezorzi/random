#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "args.h"
#include "heuristics.h"
#include "interpreter.h"

void compile(const char *path)
{
	char buffer[128] = { 0 };
	snprintf(buffer, 128, "gcc %s -o %s.out -O3", path, path);
	system(buffer);
}

void exec(const char *path)
{
	char buffer[128] = { 0 };
	snprintf(buffer, 128, "./%s.out", path);
	system(buffer);
}

int
main(int argc, char *argv[])
{

	struct settings settings = {
		.size          = 4096,
		.interpreter   = Interpterer,
		.optimizations = {
			.expr = 0,
		},
	};

	handle(argc, argv, &settings);

	unsigned char tape[settings.size];
	memset(tape, 0, settings.size);

	/* input code */
	char *code;
	int size;

	/* output file pointer */
	FILE *fp;

	if (settings.has_path_input) {
		int fd = open(settings.path_input, O_RDONLY);
		size = lseek(fd, 0, SEEK_END);
		code = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
	}

	int flags = analyze(code, size);

	switch (settings.interpreter) {

	case Interpterer:
		interpret(code, size, tape, 0);
		break;

	case Transpiler:

		if (settings.has_path_output)
			fp = fopen(settings.path_output, "w");
		else _exit(1);

		if ((flags & IS_CONSTEXPR) && settings.optimizations.expr) {
			transpile_constexpr(fp, code, size, tape, 0);
		} else if ((flags & IS_CONST) && (flags & IS_CONSTEXPR)) {
			/* do nothing */
		} else {
			transpile(fp, code, size);
		}

		fclose(fp);
		compile(settings.path_output);
		exec(settings.path_output);

		break;

	default:
		break;

	}

}
