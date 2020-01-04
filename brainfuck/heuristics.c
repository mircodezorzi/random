#include "heuristics.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>

int
analyze(char *code)
{
	/* errors/warnings */
	int row = 1;
	int col = 0;

	/* heuristics */
	int is_constexpr = 1;
	int is_empty     = 2;
	int loops        = 0;

	char c;

	/* first pass, check if loops are imbalanced */
	for (int i = 0; i < strlen(code); i++) {
		switch (code[i]) {
		case ',':
			is_constexpr = 0;
			break;
		case '.':
			is_empty = 0;
			break;
		case '[':
			loops++;
			break;
		case ']':
			loops--;
			break;
		}
	}

	if (loops == 0) return is_constexpr | is_empty;

	/* in case they are imbalanced */
	for (int i = 0; i < strlen(code); i++) {
		c = code[i];

		if (code[i] == '\n') {
			col = 0;
			row++;
		} else {
			col++;
		}

		if (code[i] == ']') {
			int j = i;
			int loop = 1;
			while (loop > 0) {
				if (j < 0) {
					printf("\033[1m%d:%d: \033[31merror:\033[0m unbalanced \033[1m']'\033[0m token\n", row, col);
					_exit(1);
					break;
				}
				c = code[--j];
				if (c == '[')
					loop--;
				else if (c == ']')
					loop++;
			}
		} else if (code[i] == '[') {
			int j = i;
			int loop = 1;
			while (loop > 0) {
				if (j > strlen(code)) {
					printf("\033[1m%d:%d: \033[31merror:\033[0m unbalanced \033[1m'['\033[0m token\n", row, col);
					_exit(1);
					break;
				}
				c = code[++j];
				if (c == '[')
					loop++;
				else if (c == ']')
					loop--;
			}
		}

	}

	return is_constexpr | is_empty;
}
