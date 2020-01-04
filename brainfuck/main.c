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
#include "repl.h"

int
main(int argc, char *argv[])
{

	struct settings settings = {
		.size          = 2048,
		.interpreter   = Interpterer,
		.optimizations = {
			.expr = 1,
			.fold = 1,
		},
	};

	handle(argc, argv, &settings);

	unsigned char tape[settings.size];
	memset(tape, 0, settings.size);

	if (settings.path_input) {
		int fd = open(settings.path_input, O_RDONLY);
		int size = lseek(fd, 0, SEEK_END);
		char *str = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
		printf("%s\n", str);
	}

	switch (settings.interpreter) {

	case Interpterer:
		printf("lunching interpreter...\n");
		analyze(argv[2]);
		interpret(argv[2], tape);
		break;

	case Transpiler:
		printf("lunching transpiler...\n");
		transpile(argv[2]);
		printf("executing output...\n");
		system("gcc output.c -o output && ./output");
		break;

	case PrettyTranspiler:
		printf("lunching pretty transpiler...\n");
		if (analyze(argv[2]) & 1) {
			printf("detected code to be constexpr...\n");
			transpile_constexpr(argv[2], tape);
		} else if ((analyze(argv[2]) & 1) && (analyze(argv[2]) & 2)) {
			printf("detected code without output, won't generate any code...\n");
		} else {
			transpile_pretty(argv[2]);
		}
		printf("executing output...\n");
		system("gcc output.c -o output && ./output");
		break;

	default:
		break;

	}


}
