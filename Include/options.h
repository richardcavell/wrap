/* wrap by Richard Cavell v1.0 */
/* January 2017 */
/* options.h */

#ifndef OPTIONS_H
#define OPTIONS_H

#include <stddef.h>

struct options_t
{
  const char *invocation;
  int files;
  size_t buffer_size;
  unsigned int line_length;
  unsigned int stops;
  int always_hyphenate;
};

const struct options_t default_options;    /* options.c */

#endif
