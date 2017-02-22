/* wrap by Richard Cavell v1.0 */
/* January-February 2017 */
/* options.c */

#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <assert.h>

#include "options.h"

#include "config.h"
#include "error.h"

static const struct options_type default_options =
{
  DEFAULT_INVOCATION,          /* config.h */
  0,       /* file_parameters */
  DEFAULT_BUFFER_SIZE,         /* config.h */
  DEFAULT_LINE_LENGTH,         /* config.h */
  DEFAULT_STOPS,               /* config.h */
  DEFAULT_ALWAYS_HYPHENATE     /* config.h */
};

int is_file(const char *arg);

static void
option_always_hyphenate(const char *param_match, const char *param_remainder,
                        struct options_type *options);

static void
option_line_break(const char *param_match, const char *param_remainder,
                  struct options_type *options);

static void
option_buffer(const char *param_match, const char *param_remainder,
              struct options_type *options);

static void
option_length(const char *param_match, const char *param_remainder,
              struct options_type *options);

static void
option_stops(const char *param_match, const char *param_remainder,
             struct options_type *options);

static void
print_help(const char *param_match, const char *param_remainder,
           struct options_type *options);

struct parameter_type
{
  const char *s;
  const char *help_text;
  void (*fn)(const char *param_match, const char *param_remainder,
             struct options_type *options);
};

static const struct parameter_type parameters[] =
{
  { "-a",                 NULL,                     option_always_hyphenate } ,
  { "--always-hyphenate", "Hyphenate instead of line-break",
                                                    option_always_hyphenate } ,
  { "-k",                 NULL,                           option_line_break } ,
  { "--line-break",       "Line-break where possible",    option_line_break } ,
  { "-b=",                NULL,                           option_buffer } ,
  { "--buffer=",          "Buffer size",                  option_buffer } ,
  { "-l=",                NULL,                           option_length } ,
  { "--length=",          "Line length",                  option_length } ,
  { "-s=",                NULL,                           option_stops } ,
  { "--stops=",           "Tab stop distance",            option_stops } ,
  { "-h",                 NULL,                           print_help } ,
  { "--help",             "Prints out this help text",    print_help } ,
  { NULL,                 NULL,                           NULL }
};

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
    int matched = 0;

    if (is_file(*argv))
    {
      options.file_parameters = 1;
      continue;
    }

    for (; !matched && param->s; ++param)
    {
      if (strncmp(param->s, *argv, strlen(param->s)) == 0)
      {
        matched = 1;
        param->fn(param->s, *argv + strlen(param->s),
                  &options);
      }
    }

    if (!matched)
      fail_msg("Error: Parameter not understood: %s\n", *argv);
  }

  return options;
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

static void
option_length(const char *param_match, const char *param_remainder,
              struct options_type *options)
{
  options->line_length = get_ui(param_remainder, param_match,
                               LENGTH_MIN, LENGTH_MAX);
}

static void
option_stops(const char *param_match, const char *param_remainder,
             struct options_type *options)
{
  options->stops = get_ui(param_remainder, param_match,
                         STOPS_MIN, STOPS_MAX);
}

static void check_param_finished(const char *param_match,
                                 const char *param_remainder);

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
option_buffer(const char *param_match, const char *param_remainder,
              struct options_type *options)
{
  assert(BUFFER_MAX <= UINT_MAX);
  options->buffer_size = get_ui(param_remainder, param_match,
                               BUFFER_MIN, BUFFER_MAX);
}

static void
print_help(const char *param_match, const char *param_remainder,
           struct options_type *options)
{
  const struct parameter_type *param = parameters;

  check_param_finished(param_match, param_remainder);

  xprintf("Wrap v1.0 by Richard Cavell\n");
  xprintf("Usage: %s [options] [filenames]\n", options->invocation);
  xprintf("Options:\n");

  for (;param->s; ++param)
  {
    const char *ht = (param->help_text) ?
                      param->help_text : HELP_TEXT_VOID;
    xprintf("  %-20s      %-40s\n", param->s, ht);
  }

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
