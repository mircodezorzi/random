#ifndef INTERPRETER_H_
#define INTERPRETER_H_

#include <stdio.h>
#include <stdlib.h>

#define clear() printf("\033[H\033[J")
#define cursorto(x,y) printf("\033[%d;%dH", (y), (x))

#define reset()         \
	do {                \
		clear();        \
		cursorto(0, 0); \
	} while(0);

#define pprint(...)                  \
do {                                 \
	fprintf(fp, "\n");               \
	for (int i = 0; i < indent; i++) \
		fprintf(fp, "\t");           \
	fprintf(fp, __VA_ARGS__);        \
} while (0)

/// \brief Interprets brainfuck code.
void interpret(
	const char    *code,
	int            size,
	unsigned char *tape,
	int            msize);

/// \brief Transpiles brainfuck code to c imploding consecutive operations and
/// formatting the c source code in a pretty way. Also doesn't discard the
/// comments.
void transpile(
	FILE       *fp,
	const char *code,
	int         size);

/// \brief Transpiles brainfuck code to c optimizing away everything except
/// output operations.
void transpile_constexpr(
	FILE*          fp,
	const char    *code,
	int            size,
	unsigned char *tape,
	int            msize);

#endif /* INTERPRETER_H_ */
