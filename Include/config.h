/* Wrap by Richard Cavell (under development) */
/* Copyright (c) 2017 Richard John Foster Cavell */
/* config.h */

#ifndef CONFIG_H
#define CONFIG_H

#include <limits.h>

  /* User-modifiable values */
#define BUFFER_MIN 100
#define DEFAULT_BUFFER_SIZE 1024
#define BUFFER_MAX ULONG_MAX

#define COLS 80
#define LENGTH_MIN 5
#define DEFAULT_LINE_LENGTH 20
#define LENGTH_MAX 200

#define STOPS_MIN 1
#define DEFAULT_STOPS 8
#define STOPS_MAX 100

#define HELP_DIVIDER1 2
#define HELP_DIVIDER2 22

/* We have to avoid buffer overruns */
#define HELP_TEXT_SCRATCH_SIZE 200

#define DEFAULT_INVOCATION "wrap"
#define DEFAULT_ALWAYS_HYPHENATE 0

#define VERSION_TEXT "Wrap (under development) by Richard Cavell\n"

typedef char ch_type;
  /* End of user-modifiable values */

#endif
