/* Wrap by Richard Cavell v1.0 */
/* Copyright (c) 2017 Richard John Foster Cavell */
/* options.c */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include <limits.h>

#include "config.h"

#include "options.h"
#include "output.h"

static const struct options_type default_options =
{
  DEFAULT_INVOCATION,          /* config.h */
  0,       /* file_parameters */
  DEFAULT_BUFFER_SIZE,         /* config.h */
  DEFAULT_LINE_LENGTH,         /* config.h */
  DEFAULT_STOPS,               /* config.h */
  DEFAULT_ALWAYS_HYPHENATE     /* config.h */
};

typedef void option_fn(const char *param_match,
                       const char *param_remainder,
                       struct options_type *options);

static option_fn option_line_length,
                 option_always_hyphenate,
                 option_line_break,
                 option_stops,
                 option_buffer_size,
                 print_help,
                 print_version;

typedef void option_ht(char *s);

static option_ht option_line_length_helptext,
                 option_always_hyphenate_helptext,
                 option_line_break_helptext,
                 option_stops_helptext,
                 option_buffer_size_helptext,
                 print_help_helptext,
                 print_version_helptext;

struct parameter_type
{
  const char *short_name, *long_name;
  option_fn *fn;
  option_ht *ht;
};

static const struct parameter_type parameters[] =
{
  { "-l=", "--line-length=",
    option_line_length,
    option_line_length_helptext } ,

  { "-a",  "--always-hyphenate",
    option_always_hyphenate,
    option_always_hyphenate_helptext } ,

  { "-k",  "--line-break",
    option_line_break,
    option_line_break_helptext } ,

  { "-s=", "--stops=",
    option_stops,
    option_stops_helptext } ,

  { "-b=", "--buffer-size=", 
    option_buffer_size,
    option_buffer_size_helptext } ,

  { "-h",  "--help",
    print_help,
    print_help_helptext } ,

  { "-v",  "--version",
    print_version,
    print_version_helptext } ,

  { NULL,  NULL,
    NULL,
    NULL }
};

static int
find_match(const char *text, const char *arg, option_fn *fn,
           struct options_type *options);

struct options_type
get_options(int argc, char *argv[])
{
  struct options_type options = default_options;

  if (argv[0])
    options.invocation = argv[0];

  options.file_parameters = 0;

  if (argc < 2)
    return options;

  while (*++argv)
  {
    const struct parameter_type *param = parameters;
    int is_filename(const char *arg); /* extern'd in options.h */
    int matched = 0;

    if (is_filename(*argv) || is_stdin(*argv))
    {
      options.file_parameters = 1;
      continue;
    }

    for (; !matched && param->short_name; ++param)
    {
      matched =    find_match(param->short_name, *argv, param->fn, &options)
                || find_match(param->long_name,  *argv, param->fn, &options);
    }

    if (!matched)
      fail_msg("Error: Parameter not understood: %s\n", *argv);
  }

  return options;
}

static int
find_match(const char *text, const char *arg, option_fn *fn,
                 struct options_type *options)
{
  int matched = 0;

  if (strncmp(text, arg, strlen(text)) == 0)
  {
    matched = 1;
    fn(text, arg + strlen(text), options);
  }

  return matched;
}


int
is_filename(const char *arg)
{
  const char hyphen = '-';
  return (arg[0] == hyphen) ? 0 : 1;
}

int
is_stdin(const char *arg)
{
  return (arg && strcmp("-", arg) == 0) ? 1 : 0;
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
