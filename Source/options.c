/* Wrap by Richard Cavell v1.0 */
/* Copyright (c) 2017 Richard John Foster Cavell */
/* options.c */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <assert.h>

#include "config.h"
#include "error.h"
#include "options.h"

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

typedef void option_ht(char *s, size_t len);

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
  option_ht *ht;
  option_fn *fn;
};

static const struct parameter_type parameters[] =
{
  { "-l=", "--line-length=",
    option_line_length_helptext,
    option_line_length } ,

  { "-a",  "--always-hyphenate",
    option_always_hyphenate_helptext,
    option_always_hyphenate } ,

  { "-k",  "--line-break",
    option_line_break_helptext,
    option_line_break } ,

  { "-s=", "--stops=",
    option_stops_helptext,
    option_stops } ,

  { "-b=", "--buffer-size=", 
    option_buffer_size_helptext,
    option_buffer_size } ,

  { "-h",  "--help",
    print_help_helptext,
    print_help } ,

  { "-v",  "--version",
    print_version_helptext,
    print_version } ,

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
    int is_file(const char *arg);
    int matched = 0;

    if (is_file(*argv))
    {
      options.file_parameters = 1;
      continue;
    }

    for (; !matched && param->short_name; ++param)
    {
      matched =    find_match(param->short_name, *argv, param->fn, &options)
                || find_match(param->long_name, *argv, param->fn, &options);
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
is_file(const char *arg)
{
  const char hyphen = '-';
  return (arg[0] == hyphen) ? 0 : 1;
}

static unsigned int get_ui(const char *param_remainder,
                           const char *param_match,
                           unsigned int min, unsigned int max);

static void check_param_finished(const char *param_match,
                                 const char *param_remainder);

static void
option_line_length(const char *param_match, const char *param_remainder,
              struct options_type *options)
{
  options->line_length = get_ui(param_remainder, param_match,
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
option_line_break(const char *param_match, const char *param_remainder,
                  struct options_type *options)
{
  check_param_finished(param_match, param_remainder);
  options->always_hyphenate = 0;
}

static void
option_stops(const char *param_match, const char *param_remainder,
             struct options_type *options)
{
  options->stops = get_ui(param_remainder, param_match,
                         STOPS_MIN, STOPS_MAX); /* macros from config.h */
}

static void
option_buffer_size(const char *param_match, const char *param_remainder,
                   struct options_type *options)
{
  assert(BUFFER_MAX <= UINT_MAX); /* macros are from config.h */
  options->buffer_size = get_ui(param_remainder, param_match,
                                BUFFER_MIN, BUFFER_MAX);
}

static void
print_version_text(void)
{
  xprintf(VERSION_TEXT);
}

static void
print_help(const char *param_match, const char *param_remainder,
           struct options_type *options)
{
  const struct parameter_type *param = parameters;
  char help_text[HELP_TEXT_SCRATCH_SIZE];

  check_param_finished(param_match, param_remainder);

  print_version_text();
  xprintf("Usage: %s [options] [filenames]\n", options->invocation);
  xprintf("Options:\n");

  for (;param->short_name; ++param)
  {
    /* void. Returns a payload in help_text */
    param->ht(help_text, sizeof(help_text));

    xprintf(" %-*sor  %-*s%s\n",
              HELP_DIVIDER1, param->short_name,
              HELP_DIVIDER2, param->long_name,
              help_text
           );
  }

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

static unsigned int
get_ui(const char *param_remainder, const char *param_match,
       unsigned int min, unsigned int max)
{
  char *endptr = NULL;
  long int l = strtol(param_remainder, &endptr, 0);

  assert(min < LONG_MAX);
  assert(max < LONG_MAX);

  if (endptr && *endptr)
    fail_msg("Error: Value given to %s must be a number\n",
                                    param_match);

  if (l < min || l > max)
    fail_msg("Error: Value to %s must be between %u and %u\n",
                              param_match, min, max);

  return (unsigned int) l;
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
option_line_length_helptext(char *s, size_t len)
{
  if (
       snprintf(s, len,
                "Line length (default %u)",
                DEFAULT_LINE_LENGTH
               )
        < 0)

    fail_msg("snprintf() failure in option_line_length_helptext()");
}

static void
option_always_hyphenate_helptext(char *s, size_t len)
{
  if (
       snprintf(s, len,
                "Fill up each line %s",
                (DEFAULT_ALWAYS_HYPHENATE) ?
                  "(default)" : "(switches off -k)"
               )
        < 0)

    fail_msg("snprintf() failure in option_always_hyphenate_helptext()");
}

static void
option_line_break_helptext(char *s, size_t len)
{
  if (
       snprintf(s, len,
                "Line-break after whole words %s",
                  DEFAULT_ALWAYS_HYPHENATE == 0 ?
                  "(default)" : "(switches off -a)"
               )
        < 0)

    fail_msg("snprintf() failure in option_line_break_helptext()");
}

static void
option_stops_helptext(char *s, size_t len)
{
  if (
       snprintf(s, len,
                "Tab stop distance (default %u)",
                DEFAULT_STOPS
               )
        < 0)

    fail_msg("snprintf() failure in option_stops_helptext()");
}

static void
option_buffer_size_helptext(char *s, size_t len)
{
  if (
       snprintf(s, len,
                "Buffer size (default %u)",
                DEFAULT_BUFFER_SIZE
               )
        < 0)

    fail_msg("snprintf() failure in option_buffer_size_helptext()");
}

static void
print_help_helptext(char *s, size_t len)
{
  if (snprintf(s, len, "Prints out this help text") < 0)
    fail_msg("snprintf() failure in option_help_helptext()");
}

static void
print_version_helptext(char *s, size_t len)
{
  if (snprintf(s, len, "Version number") < 0)
    fail_msg("snprintf() failure in option_version_helptext()");
}
