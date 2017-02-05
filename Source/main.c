/* wrap by Richard Cavell v1.0 */
/* January 2017 */
/* main.c */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <assert.h>

#include "config.h"
#include "options.h"
#include "open_file.h"

#define HYPHEN '-'

static struct options_t parse_command_line(int argc, char *argv[]);

int
main(int argc, char *argv[])
{
  extern void open_file(const char *fn, struct options_t options);

  struct options_t options = parse_command_line(argc, argv);

  if (options.files)
  {
    while (*++argv)
      if (**argv != HYPHEN)
        open_file(*argv, options);      /* open_file.c */
  }
  else
    open_file(NULL, options);           /* open_file.c */

  return EXIT_SUCCESS;
}

static struct options_t
option_always_hyphenate(const char *actual_arg, const char *param_match,
                        const char *param_remainder,
                        struct options_t options);

static struct options_t
option_line_break(const char *actual_arg, const char *param_match,
                  const char *param_remainder,
                  struct options_t options);

static struct options_t
option_buffer(const char *actual_arg, const char *param_match,
              const char *param_remainder,
              struct options_t options);

static struct options_t
option_length(const char *actual_arg, const char *param_match,
              const char *param_remainder,
              struct options_t options);

static struct options_t
option_stops(const char *actual_arg, const char *param_match,
             const char *param_remainder,
             struct options_t options);

static struct options_t
print_help(const char *actual_arg, const char *param_match,
           const char *param_remainder,
           struct options_t options);

struct parameter_t
{
  const char *s;
  const char *help_text;
  struct options_t (*fn)(const char *actual_arg, const char *param_match,
                         const char *param_remainder,
                         struct options_t options);
};

static const struct parameter_t parameters[] =
{
  { "--always-hyphenate", "Hyphenate instead of line-break",
                                                    option_always_hyphenate } ,
  { "-a",                 NULL,                     option_always_hyphenate } ,
  { "--line-break",       "Line-break where possible",    option_line_break } ,
  { "-k",                 NULL,                           option_line_break } ,
  { "--buffer=",          "Buffer size",                  option_buffer } ,
  { "-b=",                NULL,                           option_buffer } ,
  { "--length=",          "Line length",                  option_length } ,
  { "-l=",                NULL,                           option_length } ,
  { "--stops=",           "Tab stop distance",            option_stops } ,
  { "-s=",                NULL,                           option_stops } ,
  { "--help",             "Prints out this help text",    print_help } ,
  { "-h",                 NULL,                           print_help } ,
  { NULL,                 NULL,                           NULL }
};

static struct options_t
parse_command_line(int argc, char *argv[])
{
  struct options_t options = default_options;    /* options.h */
  options.invocation = argv[0];
  options.files = 0;

  if (argc < 2)
    return options;

  while (*++argv)
  {
    const struct parameter_t *param = parameters;
    int matched = 0;

    if (**argv != HYPHEN)
    {
      options.files = 1;
      continue;
    }

    for (; !matched && param->s; ++param)
    {
      if (strncmp(param->s, *argv, strlen(param->s)) == 0)
      {
        matched = 1;
        options = param->fn(*argv, param->s,
                            *argv + strlen(param->s),
                            options);
      }
    }

    if (!matched)
    {
      fprintf(stderr, "Error: Parameter not understood: %s\n", *argv);
      exit(EXIT_FAILURE);
    }
  }

  return options;
}

static unsigned int get_ui(const char *param_remainder,
                           const char *param_match,
                           unsigned int min, unsigned int max);

static struct options_t
option_length(const char *actual_arg, const char *param_match,
              const char *param_remainder,
              struct options_t options)
{
  options.line_length = get_ui(param_remainder, param_match,
                               LENGTH_MIN, LENGTH_MAX);
  return options;
}

static struct options_t
option_stops(const char *actual_arg, const char *param_match,
             const char *param_remainder,
             struct options_t options)
{
  options.stops = get_ui(param_remainder, param_match,
                         STOPS_MIN, STOPS_MAX);
  return options;
}

static void check_param_finished(const char *param_match,
                                 const char *param_remainder);

static struct options_t
option_always_hyphenate(const char *actual_arg, const char *param_match,
                        const char *param_remainder,
                        struct options_t options)
{
  check_param_finished(param_match, param_remainder);
  options.always_hyphenate = 1;
  return options;
}

static struct options_t
option_line_break(const char *actual_arg, const char *param_match,
                  const char *param_remainder,
                  struct options_t options)
{
  check_param_finished(param_match, param_remainder);
  options.always_hyphenate = 0;
  return options;
}

static struct options_t
option_buffer(const char *actual_arg, const char *param_match,
                        const char *param_remainder,
                        struct options_t options)
{
  assert(BUFFER_MAX <= UINT_MAX);
  options.buffer_size = get_ui(param_remainder, param_match,
                               BUFFER_MIN, BUFFER_MAX);
  return options;
}

static struct options_t
print_help(const char *actual_arg, const char *param_match,
           const char *param_remainder,
           struct options_t options)
{
  const struct parameter_t *param = parameters;
  const char *ht = "";

  check_param_finished(param_match, param_remainder);

  puts("Wrap v1.0 by Richard Cavell");
  printf("Usage: %s [options] [filenames]\n",
          options.invocation ? options.invocation : DEFAULT_INVOCATION);
  puts("Options:");

  for (;param->s; ++param)
  {
    ht = (param->help_text || strcmp(ht, "") == 0) ?
          param->help_text : SAME_PARAM_TEXT;
    printf("  %-20s      %-40s\n", param->s, ht);
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
  {
    fprintf(stderr, "Error: Value given to %s must be a number\n",
                     param_match);
    exit(EXIT_FAILURE);
  }

  if (l < min || l > max)
  {
    fprintf(stderr, "Error: Value to %s must be between %u and %u\n",
            param_match, min, max);
    exit(EXIT_FAILURE);
  }

  return (unsigned int) l;
}

static void
check_param_finished(const char *param_match, const char *param_remainder)
{
  if (*param_remainder)
  {
    fprintf(stderr,
            "Error: Parameter %s should not be followed by any other text\n",
            param_match);
    exit(EXIT_FAILURE);
  }
}
