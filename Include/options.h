/* Wrap by Richard Cavell v1.0 */
/* Copyright (c) 2017 Richard John Foster Cavell */
/* options.h */

#ifndef OPTIONS_H
#define OPTIONS_H

#include <stddef.h>

struct options_type
{
  const char *invocation;
  int file_parameters;  /* Either 0 (absent) or 1 (present) */
  size_t buffer_size;
  unsigned int line_length;
  unsigned int stops;
  int always_hyphenate;
};

extern int is_file(const char *arg);
extern struct options_type
         get_options(int argc, char *argv[]);

#endif
