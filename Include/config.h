/* wrap by Richard Cavell v1.0 */
/* January 2017 */
/* config.h */

#ifndef CONFIG_H
#define CONFIG_H

  /* User-modifiable values */
#define BUFFER_MIN 100
#define DEFAULT_BUFFER_SIZE 1024
#define BUFFER_MAX INT_MAX

#define LENGTH_MIN 5
#define DEFAULT_LINE_LENGTH 20
#define LENGTH_MAX 100

#define STOPS_MIN 1
#define DEFAULT_STOPS 8
#define STOPS_MAX 100

#define SAME_PARAM_TEXT "  ...same as above"

#define DEFAULT_INVOCATION "wrap"
#define DEFAULT_ALWAYS_HYPHENATE 0

typedef char ch_t;
  /* End of user-modifiable values */

#endif
