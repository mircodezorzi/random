#include "interpreter.h"

#include <stdio.h>
#include <string.h>

void
interpret(char *code, unsigned char *tape)
{
	unsigned char *ptr = tape;

	/*reset();*/

	for (int i = 0; i < strlen(code); i++) {
		char c = code[i];

		switch(c) {

		case '[': break;

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

		case '.': fwrite(ptr, 1, 1, stdout); break;
		case ',': *ptr = getchar_unlocked(); break;

		case '+': (*ptr)++; break;
		case '-': (*ptr)--; break;

		case '>': ptr++;    break;
		case '<': ptr--;    break;

		case ' ': break;
		default:
			while (code[i] != '\n' && i < strlen(code)) i++;
			break;

		}
	}

	fflush(stdout);

}

void
transpile(char *code)
{
	int indent = 1;

	FILE *fp = fopen("output.c", "w");

	fprintf(fp, "#include <stdio.h>\n");
	fprintf(fp, "#define g getchar()\n");
	fprintf(fp, "#define P(p) putchar(p)\n");
	fprintf(fp, "int main(){");
	fprintf(fp, "char m[1024]={0};");
	fprintf(fp, "char*p=m;");

	for (int i = 0; i < strlen(code); i++) {
		char c = code[i];
		int  n = 1;

		switch(c) {

		case '[':
			fprintf(fp, "while(*p){");
			indent++;
			break;

		case ']':
			indent--;
			fprintf(fp, "}");
			break;

		case '.':
			fprintf(fp, "P(*p);");
			break;

		case ',':
			fprintf(fp, "*p=G;");
			break;

		case '+':
			while (code[i + 1] == '+' && i < strlen(code)) { i++; n++; }
			if (n == 1)
				fprintf(fp, "++*p;");
			else
				fprintf(fp, "*p+=%d;", n);
			break;

		case '-':
			while (code[i + 1] == '-' && i < strlen(code)) { i++; n++; }
			if (n == 1)
				fprintf(fp, "--*p;");
			else
				fprintf(fp, "*p-=%d;", n);
			break;

		case '>':
			while (code[i + 1] == '>' && i < strlen(code)) { i++; n++; }
			if (n == 1)
				fprintf(fp, "++p;");
			else
				fprintf(fp, "p+=%d;", n);
			break;

		case '<':
			while (code[i + 1] == '<' && i < strlen(code)) { i++; n++; }
			if (n == 1)
				fprintf(fp, "--p;");
			else
				fprintf(fp, "p-=%d;", n);
			break;

		case ' ': break;
		default:
			while (code[i] != '\n' && i < strlen(code)) i++;
			break;

		}
	}

	fprintf(fp, "}");
	fclose(fp);

}

void
transpile_constexpr(char *code, unsigned char *tape)
{
	FILE *fp = fopen("output.c", "w");

	fprintf(fp, "#include <stdio.h>\n\n");
	fprintf(fp, "int main() {\n");

	/* output buffer */
	char buffer[1024] = { 0 };
	char *index = buffer;

	unsigned char *ptr = tape;

	for (int i = 0; i < strlen(code); i++) {
		char c = code[i];

		switch(c) {

		case '[': break;

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

		case ' ': break;
		default:
			while (code[i] != '\n' && i < strlen(code)) i++;
			break;

		}
	}

	fprintf(fp, "\tprintf(R\"(%s)\");\n}", buffer);
	fclose(fp);

}

void
transpile_pretty(char *code)
{
	int indent = 1;

	FILE *fp = fopen("output.c", "w");

	fprintf(fp, "#include <stdio.h>\n\n");
	fprintf(fp, "int main() {\n");
	pprint("char array[1024] = { 0 };");
	pprint("char *ptr = array;\n");

	for (int i = 0; i < strlen(code); i++) {
		char c = code[i];
		int  n = 1;

		switch(c) {

		case '[':
			pprint("while (*ptr) {");
			indent++;
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
			while (code[i + 1] == '+' && i < strlen(code)) { i++; n++; }
			if (n == 1)
				pprint("++*ptr;");
			else
				pprint("*ptr += %d;", n);
			break;

		case '-':
			while (code[i + 1] == '-' && i < strlen(code)) { i++; n++; }
			if (n == 1)
				pprint("--*ptr;");
			else
				pprint("*ptr -= %d;", n);
			break;

		case '>':
			while (code[i + 1] == '>' && i < strlen(code)) { i++; n++; }
			if (n == 1)
				pprint("++ptr;");
			else
				pprint("ptr += %d;", n);
			break;

		case '<':
			while (code[i + 1] == '<' && i < strlen(code)) { i++; n++; }
			if (n == 1)
				pprint("--ptr;");
			else
				pprint("ptr -= %d;", n);
			break;

		case ' ': break;

		default:
			fprintf(fp, " // ");
			while (code[i] != '\n' && i < strlen(code)) {
				fprintf(fp, "%c", code[i]);
				i++;
			}
			break;

		}
	}

	fprintf(fp, "\n}");

	fclose(fp);
}
