#ifndef HEURISTICS_H_
#define HEURISTICS_H_

#define IS_CONSTEXPR 0x1
#define IS_CONST     0x2

static inline void unbalanced_token(char *code,
									int   row,
									int   col,
									int   absolute,
									char  token);

/// \brief Checks if code is constexpr or has unbalanced loops.
int analyze(char *code, int size);

#endif /* HEURISTICS_H_ */
