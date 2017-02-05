/* wrap by Richard Cavell v1.0 */
/* January 2017 */
/* options.c */

#include <stdio.h>
#include "config.h"
#include "options.h"

const struct options_t default_options =
{
  NULL,    /* invocation */
  0,       /* files */
  DEFAULT_BUFFER_SIZE,         /* config.h */
  DEFAULT_LINE_LENGTH,         /* config.h */
  DEFAULT_STOPS,               /* config.h */
  DEFAULT_ALWAYS_HYPHENATE     /* config.h */
};
