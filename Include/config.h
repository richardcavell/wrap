/* Wrap by Richard Cavell v1.0 */
/* January-February 2017 */
/* config.h */

#ifndef CONFIG_H
#define CONFIG_H

#include <limits.h>

  /* User-modifiable values */
#define BUFFER_MIN 100
#define DEFAULT_BUFFER_SIZE 1024
#define BUFFER_MAX INT_MAX

#define COLS 80
#define LENGTH_MIN 5
#define DEFAULT_LINE_LENGTH 20
#define LENGTH_MAX 100

#define STOPS_MIN 1
#define DEFAULT_STOPS 8
#define STOPS_MAX 100

#define HELP_DIVIDER1 5
#define HELP_DIVIDER2 22

/* Right edge - left edge - 5 for prettiness + 2 for the \n and \0 */
#define HELP_TEXT_SCRATCH_SIZE (COLS - HELP_DIVIDER2 - 5 + 2)

#define DEFAULT_INVOCATION "wrap"
#define DEFAULT_ALWAYS_HYPHENATE 0

#define VERSION_TEXT "Wrap v1.0 by Richard Cavell\n"

typedef char ch_type;
  /* End of user-modifiable values */

#endif
