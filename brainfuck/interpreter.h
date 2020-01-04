#ifndef INTERPRETER_H_
#define INTERPRETER_H_

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
void interpret(char *code, unsigned char *tape);

/// \brief Transpiles brainfuck code to c imploding consecutive operations.
void transpile(char *code);

/// \brief Transpiles brainfuck code to c optimizing away everything except
/// output operations.
void transpile_constexpr(char *code, unsigned char *tape);

/// \brief Transpiles brainfuck code to c imploding consecutive operations and
/// formatting the c source code in a pretty way. Also doesn't discard the
/// comments.
void transpile_pretty(char *code);

#endif /* INTERPRETER_H_ */
