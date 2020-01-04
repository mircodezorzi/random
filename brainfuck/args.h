#ifndef ARGS_H_
#define ARGS_H_

struct settings {

	/* number of cells */
	long size;

	/* type of interpreter */
	enum {
		Interpterer,
		Transpiler,
		PrettyTranspiler
	} interpreter;

	char path_input[512];
	char path_output[512];

	struct {
		unsigned int expr : 1; /* constexpr */
		unsigned int fold : 1; /* consecutive operations */
	} optimizations;

};

/// \brief handle arguments
void handle(int argc, char *argv[], struct settings *settings);

/// \brief Display help message
void usage();

#endif /* ARGS_H_ */
