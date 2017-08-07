/* Wrap by Richard Cavell (under development) */
/* Copyright (c) 2017 Richard John Foster Cavell */
/* options.c */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <limits.h>
#include <errno.h>

#include "options.h"

#include "config.h"
#include "output.h"

static const struct options_type default_options =
{
  DEFAULT_INVOCATION,          /* config.h */
  NULL,    /* filenames */
  DEFAULT_BUFFER_SIZE,         /* config.h */
  DEFAULT_LINE_LENGTH,         /* config.h */
  DEFAULT_STOPS,               /* config.h */
  DEFAULT_ALWAYS_HYPHENATE     /* config.h */
};

typedef void option_fn     (struct options_type *options);
typedef void option_fn_ul  (struct options_type *options,
                              unsigned long l);
typedef void option_ht     (char *s);

static option_fn
                 always_hyphenate,
                 line_break,
                 print_help,
                 print_version;

static option_fn_ul
                 ul_line_length,
                 ul_stops,
                 ul_buffer_size;

static option_ht
                 always_hyphenate_helptext,
                 line_break_helptext,
                 help_helptext,
                 version_helptext,

                 line_length_helptext,
                 stops_helptext,
                 buffer_size_helptext;

struct parameter_type
{
  const char     *short_name, *long_name;
  option_fn      *fn;
  option_fn_ul   *fn_ul;
  option_ht      *ht;
};

static const struct parameter_type parameters[] =
{
  { "l",  "line-length",
    NULL,
    option_line_length,
    option_line_length_helptext,
  } ,

  { "a",  "always-hyphenate",
    option_always_hyphenate,
    NULL,
    option_always_hyphenate_helptext,
  } ,

  { "k",  "line-break",
    option_line_break,
    NULL,
    option_line_break_helptext,
  } ,

  { "s",  "stops",
    NULL,
    option_stops,
    option_stops_helptext,
  } ,

  { "b",  "buffer-size",
    NULL,
    option_buffer_size,
    option_buffer_size_helptext,
  } ,

  { "h",  "help",
    print_help,
    NULL,
    print_help_helptext,
  } ,

  { "v",  "version",
    print_version,
    NULL,
    print_version_helptext,
  } ,

  { NULL,  NULL,
    NULL,
    NULL,
    NULL
  }
};

static void
free_filenames(void);

const char *(*filenames)[];    /* declaration only - definition is below */

struct options_type
get_options(int argc, char *argv[])
{
  struct options_type options = default_options;
  const char **filenames_insert_pt = NULL;

  options.filenames = NULL;

  if (argv[0])
    options.invocation = argv[0];

  if (argc >= 2)
    while (*++argv)
    {



      if (is_filename(*argv) || is_stdin(*argv))
      {
        if (options.filenames == NULL)
        {
          filenames =
            options.filenames =
              malloc((size_t) argc * sizeof(const char *));

          if (atexit(free_filenames))
            fail_msg("Couldn't register options atexit callback");

          filenames_insert_pt = *options.filenames;
        }

        *filenames_insert_pt++ = *argv;
      }
      else
      {
        find_match(*argv, &options);
        if (options.filenames == 0)
          --argc;
      }
    }

  if (filenames_insert_pt)
     *filenames_insert_pt = NULL;

  return options;
}

static void
find_match(const char *arg, struct options_type *options)
{
  const struct parameter_type *param = parameters;
  int matched = 0;

  for (; !matched && param->short_name; ++param)
  {
    matched =    attempt_match(param->short_name, arg, param->fn, options)
              || attempt_match(param->long_name,  arg, param->fn, options);
  }

  if (!matched)
    fail_msg("Error: Parameter not understood: %s\n", arg);
}

static int
attempt_match(const char *param_text, const char *actual_arg,
              option_fn *fn, struct options_type *options)
{
  int matched = 0;

  if (strncmp(param_text, actual_arg, strlen(param_text)) == 0)
  {
    matched = 1;
    fn(param_text, actual_arg + strlen(param_text), options);
  }

  return matched;
}

int
is_filename(const char *arg)
{
  const char hyphen = '-';
  return (arg && arg[0] != hyphen);
}

int
is_stdin(const char *arg)
{
  const char hyphen[] = "-";
  return (arg && strcmp(hyphen, arg) == 0);
}

const char *(*filenames)[] = NULL;

static void free_filenames(void)
{
  free(filenames);
  filenames = NULL;
}

static unsigned long int get_ul(const char *param_remainder,
                                const char *param_match,
                                unsigned long int min, unsigned long int max);

static void check_param_finished(const char *param_match,
                                 const char *param_remainder);

static void
option_line_length(const char *param_match,
                   const char *param_remainder,
                   struct options_type *options)
{
  assert(LENGTH_MIN > 0 && LENGTH_MAX <= UINT_MAX);
  options->line_length = (unsigned int)
                         get_ul(param_remainder, param_match,
                         LENGTH_MIN, LENGTH_MAX); /* macros from config.h */
}

static void
option_always_hyphenate(const char *param_match,
                        const char *param_remainder,
                        struct options_type *options)
{
  check_param_finished(param_match, param_remainder);
  options->always_hyphenate = 1;
}

static void
option_line_break(const char *param_match,
                  const char *param_remainder,
                  struct options_type *options)
{
  check_param_finished(param_match, param_remainder);
  options->always_hyphenate = 0;
}

static void
option_stops(const char *param_match, const char *param_remainder,
             struct options_type *options)
{
  assert(STOPS_MIN > 0 && STOPS_MAX < UINT_MAX);
  options->stops = (unsigned int)
                   get_ul(param_remainder, param_match,
                   STOPS_MIN, STOPS_MAX); /* macros from config.h */
}

static void
option_buffer_size(const char *param_match, const char *param_remainder,
                   struct options_type *options)
{
  assert(BUFFER_MIN > 0 && BUFFER_MAX <= ULONG_MAX);
  options->buffer_size = get_ul(param_remainder, param_match,
                                BUFFER_MIN, BUFFER_MAX); /* from config.h */
}

static void
print_version_text(void)
{
  xprintf(VERSION_TEXT); /* VERSION_TEXT is in config.h */
}

static void
print_help(const char *param_match, const char *param_remainder,
           struct options_type *options)
{
  const struct parameter_type *param = parameters;
  char help_text[HELP_TEXT_SCRATCH_SIZE]; /* config.h */

  check_param_finished(param_match, param_remainder);

  print_version_text();
  xprintf("Usage: %s [options] [filenames]\n", options->invocation);
  xprintf("Options:\n");

  for (;param->short_name; ++param)
  {
    /* void. Returns a payload in help_text */
    param->ht(help_text);

    /* Macros are in config.h */
    xprintf(" %-*sor  %-*s%s\n",
              HELP_DIVIDER1, param->short_name,
              HELP_DIVIDER2, param->long_name,
              help_text
           );
  }

  xprintf("If no filenames are given, standard input will be used.\n");
  xprintf("If filenames are given, use a single - to read from standard input.\n");

  exit(EXIT_SUCCESS);
}

static void
print_version(const char *param_match, const char *param_remainder,
              struct options_type *options)
{
  /* Suppress compiler warnings about unused parameter */
  (void) options;

  check_param_finished(param_match, param_remainder);
  print_version_text();
  exit(EXIT_SUCCESS);
}

static unsigned long int
get_ul(const char *param_remainder, const char *param_match,
       unsigned long int min, unsigned long int max)
{
  char *endptr = NULL;
  int neg = (strtol(param_remainder, NULL, 0) < 0);
  unsigned long int l = strtoul(param_remainder, &endptr, 0);

  /* This won't catch all such errors, but it's still worth doing */
  if (neg)
    fail_msg("Error: Value given to %s must be positive\n",
                                    param_match);

  if (endptr && *endptr)
    fail_msg("Error: Value given to %s must be a number\n",
                                    param_match);

  if (l < min || l > max)
    fail_msg("Error: Value to %s must be between %u and %u\n",
                              param_match,       min,   max);

  if ((l == 0) && errno)
    fail_msg("Error: Cannot understand the number given to %s. Error %i\n",
                                                  param_match,       errno);

  return l;
}

static void
check_param_finished(const char *param_match, const char *param_remainder)
{
  if (*param_remainder)
    fail_msg(
      "Error: Parameter %s should not be followed by any other text\n",
                        param_match);
}

static void
option_line_length_helptext(char *s)
{
  if (
       sprintf(s,
                "Line length (default %u)",
                DEFAULT_LINE_LENGTH
               )
        < 0)

    fail_msg("sprintf() failure in option_line_length_helptext()");
}

static void
option_always_hyphenate_helptext(char *s)
{
  if (
       sprintf(s,
                "Fill up each line %s",
                (DEFAULT_ALWAYS_HYPHENATE) ?
                  "(default)" : "(switches off -k)"
               )
        < 0)

    fail_msg("sprintf() failure in option_always_hyphenate_helptext()");
}

static void
option_line_break_helptext(char *s)
{
  if (
       sprintf(s,
                "Line-break after whole words %s",
                  DEFAULT_ALWAYS_HYPHENATE == 0 ?
                  "(default)" : "(switches off -a)"
               )
        < 0)

    fail_msg("sprintf() failure in option_line_break_helptext()");
}

static void
option_stops_helptext(char *s)
{
  if (
       sprintf(s,
                "Tab stop distance (default %u)",
                DEFAULT_STOPS
               )
        < 0)

    fail_msg("sprintf() failure in option_stops_helptext()");
}

static void
option_buffer_size_helptext(char *s)
{
  if (
       sprintf(s,
                "Buffer size (default %lu)",
                (unsigned long) DEFAULT_BUFFER_SIZE
               )
        < 0)

    fail_msg("sprintf() failure in option_buffer_size_helptext()");
}

static void
print_help_helptext(char *s)
{
  if (sprintf(s, "Prints out this help text") < 0)
    fail_msg("sprintf() failure in option_help_helptext()");
}

static void
print_version_helptext(char *s)
{
  if (sprintf(s, "Version number") < 0)
    fail_msg("sprintf() failure in option_version_helptext()");
}
