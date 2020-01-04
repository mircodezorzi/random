#include "interpreter.h"

#include <stdio.h>
#include <string.h>

#include <inttypes.h>
#include <math.h>
#include <stdio.h>
#include <time.h>

void
interpret(const char    *code,
		  int            size,
		  unsigned char *tape,
		  int            msize)
{
	unsigned char *ptr = tape;

	for (int i = 0; i < size; i++) {
		char c = code[i];

		switch(c) {

		case '[':
			if (!*ptr) {
				int loop = 1;
				while (loop > 0) {
					c = code[++i];
					if (c == '[')
						loop++;
					else if (c == ']')
						loop--;
				}
			}
			break;

		case ']':
			if (*ptr) {
				int loop = 1;
				while (loop > 0) {
					c = code[--i];
					if (c == '[')
						loop--;
					else if (c == ']')
						loop++;
				}
			}
			break;

		case '.':
			/*if (verbose)*/
				/*fwrite(ptr, 1, 1, stdout);*/
			printf("%c", *ptr);
			break;
		case ',':
			/*if (verbose)*/
				/*fflush(stdout);*/
			*ptr = getchar();
			break;

		case '+': (*ptr)++; break;
		case '-': (*ptr)--; break;

		case '>': ptr++;    break;
		case '<': ptr--;    break;

		case ' ': break;
		default:
			while (code[i] != '\n' && i < size) i++;
			break;

		}
	}

	/*if (verbose)*/
		fflush(stdout);
}

void
transpile(FILE       *fp,
		  const char *code,
		  int         size)
{

	int  indent = 1;

	fprintf(fp, "#include <stdio.h>\n\n");
	fprintf(fp, "int main() {\n");
	pprint("char array[1024] = { 0 };");
	pprint("char *ptr = array;\n");

	for (int i = 0; i < size; i++) {
		char c = code[i];
		int  n = 1;

		switch(c) {

		case '[':

			if (code[i + 1] == '-' && code[i + 2] == ']') {
				pprint("*ptr = 0;");
				i += 2;
			}
			else {
				pprint("while (*ptr) {");
				indent++;
			}
			break;

		case ']':
			indent--;
			pprint("}");
			break;

		case '.':
			pprint("putchar(*ptr);");
			break;

		case ',':
			pprint("*ptr = getchar();");
			break;

		case '+':
			while (code[i + 1] == '+' && i < size) { i++; n++; }
			if (n == 1)
				pprint("++*ptr;");
			else
				pprint("*ptr += %d;", n);
			break;

		case '-':
			while (code[i + 1] == '-' && i < size) { i++; n++; }
			if (n == 1)
				pprint("--*ptr;");
			else
				pprint("*ptr -= %d;", n);
			break;

		case '>':
			while (code[i + 1] == '>' && i < size) { i++; n++; }
			if (n == 1)
				pprint("++ptr;");
			else
				pprint("ptr += %d;", n);
			break;

		case '<':
			while (code[i + 1] == '<' && i < size) { i++; n++; }
			if (n == 1)
				pprint("--ptr;");
			else
				pprint("ptr -= %d;", n);
			break;

		case '\n': break;
		case ' ': break;

		default:
			fprintf(fp, " // ");
			while (code[i] != '\n' && i < size) {
				fprintf(fp, "%c", code[i]);
				i++;
			}
			break;

		}
	}

	fprintf(fp, "\n}");

}

void
transpile_constexpr(FILE          *fp,
					const char    *code,
					int            size,
					unsigned char *tape,
					int            msize)
{

	fprintf(fp, "#include <stdio.h>\n\n");
	fprintf(fp, "int main() {\n");

	/* output buffer */
	char buffer[4096 * 4] = { 0 };
	char *index = buffer;

	unsigned char *ptr = tape;

	for (int i = 0; i < size; i++) {
		char c = code[i];

		switch(c) {

		case '[':
			if (!*ptr) {
				int loop = 1;
				while (loop > 0) {
					c = code[++i];
					if (c == '[')
						loop++;
					else if (c == ']')
						loop--;
				}
			}
			break;

		case ']':
			if (*ptr) {
				int loop = 1;
				while (loop > 0) {
					c = code[--i];
					if (c == '[')
						loop--;
					else if (c == ']')
						loop++;
				}
			}
			break;

		case '.':
			*index = *ptr;
			index++;
			break;

		case '+': (*ptr)++; break;
		case '-': (*ptr)--; break;

		case '>': ptr++;    break;
		case '<': ptr--;    break;

		case '\n': break;
		case ' ': break;
		default:
			while (code[i] != '\n' && i < size) i++;
			break;

		}
	}

	fprintf(fp, "\tprintf(R\"(%s)\");\n}", buffer);

}
