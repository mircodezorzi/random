#include "heuristics.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>

static inline void
unbalanced_token(char *code,
				 int   row,
				 int   col,
				 int   absolute,
				 char  token)
{
	int before = 3;

	printf("\033[1m%d:%d: \033[31merror:\033[0m unbalanced \033[1m'%c'\033[0m token\n", row, col, token);
	if (col > before)
		printf("%.*s\n", 10, code + absolute - before);
	else
		printf("%.*s\n", 10, code + absolute - 1);
	for (int k = 0; k < before - 1 && col > before; k++)
		printf(" ");
	puts("\033[1;31m^\033[0m");
	_exit(1);
}

int
analyze(char *code, int size)
{
	/* errors/warnings */
	int row = 1;
	int col = 0;
	int absolute = 0;

	/* heuristics */
	int is_constexpr = 1;
	int is_const     = 1;
	int loops        = 0;

	char c;

	/* first pass, check if loops are imbalanced */
	for (int i = 0; i < size; i++) {
		switch (code[i]) {
		case ',':
			is_constexpr = 0;
			break;
		case '.':
			is_const = 0;
			break;
		case '[':
			loops++;
			break;
		case ']':
			loops--;
			break;
		}
	}

	if (loops == 0)
		return (is_constexpr && IS_CONSTEXPR) | (is_const & IS_CONST);

	/* in case they are imbalanced, potentially O(n^3) */
	for (int i = 0; i < size; i++) {
		c = code[i];

		if (code[i] == '\n') {
			col = 0;
			row++;
			absolute++;
		} else {
			col++;
			absolute++;
		}

		if (code[i] == ']') {
			int j    = i;
			int loop = 1;
			while (loop > 0) {
				if (j < 0)
					unbalanced_token(code, row, col, absolute, ']');
				c = code[--j];
				if (c == '[')
					loop--;
				else if (c == ']')
					loop++;
			}
		} else if (code[i] == '[') {
			int j    = i;
			int loop = 1;
			while (loop > 0) {
				if (j > size)
					unbalanced_token(code, row, col, absolute, '[');
				c = code[++j];
				if (c == '[')
					loop++;
				else if (c == ']')
					loop--;
			}
		}

	}

	return (is_constexpr && IS_CONSTEXPR) | (is_const & IS_CONST);

}
