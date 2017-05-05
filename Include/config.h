/* Wrap by Richard Cavell v1.0 */
/* January-February 2017 */
/* config.h */

#ifndef CONFIG_H
#define CONFIG_H

#include <limits.h>

#define _makestr(a) #a
#define makestr(a) _makestr(a)

  /* User-modifiable values */
#define BUFFER_MIN 100
#define DEFAULT_BUFFER_SIZE 1024
#define DEFAULT_BUFFER_SIZE_TEXT makestr(DEFAULT_BUFFER_SIZE)
#define BUFFER_MAX INT_MAX

#define LENGTH_MIN 5
#define DEFAULT_LINE_LENGTH 20
#define DEFAULT_LINE_LENGTH_TEXT makestr(DEFAULT_LINE_LENGTH)
#define LENGTH_MAX 100

#define STOPS_MIN 1
#define DEFAULT_STOPS 8
#define DEFAULT_STOPS_TEXT makestr(DEFAULT_STOPS)
#define STOPS_MAX 100

#define HELP_DIVIDER1 5
#define HELP_DIVIDER2 22

#define DEFAULT_INVOCATION "wrap"
#define DEFAULT_ALWAYS_HYPHENATE 0

#define VERSION_TEXT "Wrap v1.0 by Richard Cavell\n"

typedef char ch_type;
  /* End of user-modifiable values */

#endif
