/* Wrap by Richard Cavell v1.0 */
/* Copyright (c) 2017 Richard John Foster Cavell */
/* options.h */

#ifndef OPTIONS_H
#define OPTIONS_H

#include <stddef.h>

struct options_type
{
  const char *invocation;  /* The name of this program, from the command line */
  int file_parameters;     /* Either 0 (absent) or 1 (present) */
  size_t buffer_size;
  unsigned int line_length;
  unsigned int stops;
  int always_hyphenate;    /* Either 0 or 1 */
};

/* These two return 0 or 1 */
extern int is_filename(const char *arg);
extern int is_stdin(const char *arg);

/*
   Call this function below once, at the start of the program,
   with the command line arguments.
*/

extern struct options_type
get_options(int argc, char *argv[]);

#endif
