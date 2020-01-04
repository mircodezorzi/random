#ifndef ARGS_H_
#define ARGS_H_

static inline long scan_number(char *buffer);

static inline void scan_path(char *in, char* out, int n);

struct settings {

	/* number of cells */
	long size;

	/* type of interpreter */
	enum {
		Interpterer,
		Transpiler,
	} interpreter;

	char path_input[512];
	char path_output[512];

	int has_path_input;
	int has_path_output;

	struct {
		unsigned int expr : 1; /* constexpr */
	} optimizations;

};

/// \brief handle arguments
void handle(int argc, char *argv[], struct settings *settings);

/// \brief Display help message
void usage();

#endif /* ARGS_H_ */
