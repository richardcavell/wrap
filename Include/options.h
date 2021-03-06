/* Wrap by Richard Cavell (under development) */
/* Copyright (c) 2017 Richard John Foster Cavell */
/* options.h */

#ifndef OPTIONS_H
#define OPTIONS_H

#include <stddef.h>

struct options_type
{
  const char *invocation;  /* The name of this program, from the command line */
  const char *(*filenames)[];
  size_t buffer_size;
  unsigned int line_length;
  unsigned int stops;      /* Tab stops are every (this number) of chars */
  int always_hyphenate;    /* Either 0 or 1 */
};

/* This returns 0 or 1 */
extern int is_stdin(const char *arg);

/*
   Call this function below once, at the start of the program,
   with the command line arguments.
*/

extern struct options_type
get_options(int argc, const char *argv[]);

#endif
