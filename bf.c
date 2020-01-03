#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	if (argc < 2) _exit(1);

	unsigned char memory[1024] = { 0 };
	unsigned char *ptr = memory;

	for (int i = 0; argv[1][i] != '\0'; i++) {
		char c = argv[1][i];

		switch(c) {

		case '[':
			if (!*ptr) {
				int loop = 1;
				while (loop > 0) {
					c = argv[1][++i];
					if (c == '[')
						loop--;
					else if (c == ']')
						loop++;
				}
			}
			break;

		case ']':
			if (*ptr) {
				int loop = 1;
				while (loop > 0) {
					c = argv[1][--i];
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

		default: break;

		}
	}

	fflush(stdout);

	for (int i = 0; i < 80; i++) {
		if (memory[i] > ' ' && memory[i] < '~')
			printf("%c", memory[i]);
		else
			printf(".");
		if ((i + 1) % 40 == 0) puts("");
	}

	FILE *fp = fopen("output.c", "w");

	fprintf(fp, "#include <stdio.h>\n");
	fprintf(fp, "int main() {\n");
	fprintf(fp, "char array[1024] = { 0 };\n");
	fprintf(fp, "char *ptr = array;\n");

	for (int i = 0; argv[1][i] != '\0'; i++) {
		char c = argv[1][i];

		switch(c) {

		case '[':
			fprintf(fp, "while (*ptr) {\n");
			break;

		case ']':
			fprintf(fp, "}\n");
			break;

		case '.':
			fprintf(fp, "putchar(*ptr);\n");
			break;

		case ',':
			fprintf(fp, "*ptr = getchar();\n");
			break;

		case '+':
			fprintf(fp, "++*ptr;\n");
			break;

		case '-':
			fprintf(fp, "--*ptr;\n");
			break;

		case '>':
			fprintf(fp, "++ptr;\n");
			break;

		case '<':
			fprintf(fp, "--ptr;\n");
			break;

		default: break;

		}
	}

	fprintf(fp, "}");

	fclose(fp);

	system("gcc output.c -o output && ./output");

}
