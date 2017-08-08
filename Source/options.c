/* Wrap by Richard Cavell (under development) */
/* Copyright (c) 2017 Richard John Foster Cavell */
/* options.c */

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
typedef void option_ht     (void);
typedef void option_ht_ul  (unsigned long int deflt);

static option_fn
                     always_hyphenate,
                     line_break,
                     help,
                     version;

static option_fn_ul
                     ul_line_length,
                     ul_stops,
                     ul_buffer_size;

static option_ht
                     ht_always_hyphenate,
                     ht_line_break,
                     ht_help,
                     ht_version;

static option_ht_ul
                     ht_line_length,
                     ht_stops,
                     ht_buffer_size;

struct parameter_type
{
  const char         *short_name;    const char *long_name;
  option_fn          *fn;          option_ht    *ht;
  option_fn_ul       *fn_ul;       option_ht_ul *ht_ul;
  unsigned long int  min;       /* any macros will be in config.h */
  unsigned long int  deflt;     /* same here */
  unsigned long int  max;       /* same here */
};

static const struct parameter_type parameters[] =
{
  {
                             "l",    "line-length",
                            NULL,  NULL,
                  ul_line_length,  ht_line_length,
                      LENGTH_MIN,
             DEFAULT_LINE_LENGTH,
                      LENGTH_MAX
   } ,

  {
                             "a",    "always-hyphenate",
                always_hyphenate,  ht_always_hyphenate
  } ,

  {
                             "k",    "line-break",
                      line_break,  ht_line_break
  } ,

  {
                             "s",    "stops",
                            NULL,  NULL,
                        ul_stops,  ht_stops,
                       STOPS_MIN,
                   DEFAULT_STOPS,
                       STOPS_MAX
  } ,

  {
                             "b",    "buffer-size",
                            NULL,  NULL,
                  ul_buffer_size,  ht_buffer_size,
                      BUFFER_MIN,
             DEFAULT_BUFFER_SIZE,
                      BUFFER_MAX
  } ,

  {
                             "h",    "help",
                            help,  ht_help
  } ,

  {
                             "v",    "version",
                         version,  ht_version
  } ,

  {
                            NULL,  NULL,
                            NULL,  NULL,
                            NULL,  NULL,
                               0,
                               0,
                               0
  }
};

static void
check_config(void)
{
  assert(BUFFER_MIN >= 5);
  assert(DEFAULT_BUFFER_SIZE >= BUFFER_MIN);
  assert(DEFAULT_BUFFER_SIZE <= BUFFER_MAX);
  assert(BUFFER_MAX <= ULONG_MAX);

  assert(LENGTH_MIN >= 5);
  assert(DEFAULT_LINE_LENGTH >= LENGTH_MIN);
  assert(DEFAULT_LINE_LENGTH <= LENGTH_MAX);
  assert(LENGTH_MAX <= UINT_MAX);

  assert(STOPS_MAX < UINT_MAX);
}

static int  starts_with(const char *s, const char *t);

static void long_opt(const char *option_text, const char **pargv[],
                     struct options_type *options);

static void short_opt(const char *option_text, const char **pargv[],
                      struct options_type *options);

static void push_filename(const char *s, int argc);

const char  *(*filenames)[];    /* declaration only - definition is below */

struct options_type
get_options(int argc, const char *argv[])
{
  struct options_type options = default_options;

  const char lp[] = "--";
  const char sp[] = "-";

  check_config();

  options.invocation = argv[0] ? argv[0] : DEFAULT_INVOCATION;

  if (argc >= 2)
  {
    while (*++argv)
    {
           if (starts_with(argv[0], lp))
           {
             if (filenames == NULL)
               --argc;
             long_opt(&argv[0][strlen(lp)], &argv, &options);
           }
      else if (is_stdin(argv[0]))
           {
             push_filename(argv[0], argc);
           }
      else if (starts_with(argv[0], sp))
           {
             if (filenames == NULL)
               --argc;
             short_opt(&argv[0][strlen(sp)], &argv, &options);
           }
      else
           {
                 push_filename(argv[0], argc);
           }
    }
  }

  options.filenames = filenames;

  return options;
}

static int
starts_with(const char *s, const char *t)
{
  return strncmp(s, t, strlen(t)) == 0;
}

static void ul_param(struct options_type *options,
                     const struct parameter_type *param,
                     const char *ul_text, const char *error_s);

static void
long_opt(const char *option_text, const char **pargv[],
         struct options_type *options)
{
  const struct parameter_type *param = parameters;
  int matched = 0;

  for (; !matched && param->long_name; ++param)
  {
    if (strncmp(param->long_name, option_text,
                strlen(param->long_name)) == 0)
    {
      char *error_s;

      matched = 1;
      if (param->fn)
          param->fn(options);
      else
      {
        const char *ul = NULL;

             if (option_text[strlen(param->long_name)] == '=')
           ul = &option_text[strlen(param->long_name) + 1];
        else if (option_text[strlen(param->long_name)] != '\0')
           ul = &option_text[strlen(param->long_name)];
        else
        {
             ++(*pargv);
          ul = (*pargv)[0];
        }

        error_s = malloc(strlen(param->long_name) + 3);
        strcpy(error_s, "--");
        strcat(error_s, param->long_name);
        ul_param(options, param, ul, error_s);
        free(error_s);
      }
    }
  }

  if (!matched)
    fail_msg("Error: Parameter not understood: --%s\n", option_text);
}

static void
short_opt(const char *option_text, const char **pargv[],
         struct options_type *options)
{
  const struct parameter_type *param = parameters;
  int matched = 0;

  for (; !matched && param->short_name; ++param)
  {
    if (strncmp(param->short_name, option_text,
                strlen(param->short_name)) == 0)
    {
      char *error_s;

      matched = 1;
      if (param->fn)
      {
         param->fn(options);

         if (option_text[strlen(param->short_name)])
             short_opt(option_text + strlen(param->short_name),
                     pargv, options);
      }
      else
      {
        const char *ul = NULL;

             if (option_text[strlen(param->short_name)] == '=')
           ul = &option_text[strlen(param->short_name) + 1];
        else if (option_text[strlen(param->short_name)] != '\0')
           ul = &option_text[strlen(param->short_name)];
        else
        {
             ++(*pargv);
          ul = (*pargv)[0];
        }

        error_s = malloc(strlen(param->long_name) + 2);
        strcpy(error_s, "-");
        strcat(error_s, param->short_name);
        ul_param(options, param, ul, error_s);
        free(error_s);
      }
    }
  }

  if (!matched)
    fail_msg("Error: Option not understood : -%s\n", option_text);
}

static void
ul_param(struct options_type *options,
         const struct parameter_type *param,
         const char *ul_text, const char *error_s)
{
  int neg;
  char *endptr = NULL;
  unsigned long int ul;

  if (ul_text == NULL || ul_text[0] == '\0')
    fail_msg("Error: %s requires a value\n", error_s);

  neg = (strtol(ul_text, NULL, 0) < 0);

  /* This won't catch all such errors, but it's still worth doing */
  if (neg)
    fail_msg("Error: Value given to %s must be positive\n",
                                    error_s);

  ul  =  strtoul(ul_text, &endptr, 0);

  if (endptr && *endptr)
    fail_msg("Error: Value given to %s must be a number\n",
                                    error_s);

  if (ul < param->min || ul > param->max)
    fail_msg("Error: Value to %s must be between %u and %u\n",
                              error_s,   param->min,    param->max);

  if ((ul == 0) && errno)
    fail_msg("Error: Cannot understand the number given to %s. Error %i\n",
                                                      error_s,    errno);

  param->fn_ul(options, ul);
}

int
is_stdin(const char *arg)
{
  const char hyphen[] = "-";
  return (arg && strcmp(hyphen, arg) == 0);
}

const char *(*filenames)[] = NULL;

static void
free_filenames(void);

static const char *(*allocate_filenames(int argc))[]
{
  filenames = calloc((size_t) argc, sizeof(const char *));

  if (filenames == NULL)
    fail_msg("Error: Couldn't allocate filenames array()\n");

  if (atexit(free_filenames))
    fail_msg("Error: Couldn't register options callback\n");

  return filenames;
}

static void push_filename(const char *s, int argc)
{
  static const char **filenames_insert_pt = NULL;

  if (filenames_insert_pt == NULL)
      filenames_insert_pt = *(allocate_filenames(argc));

  filenames_insert_pt++[0] = s;
}

static void free_filenames(void)
{
  free(filenames);
  filenames = NULL;
}

static void
always_hyphenate(struct options_type *options)
{
  options->always_hyphenate = 1;
}

static void
line_break(struct options_type *options)
{
  options->always_hyphenate = 0;
}

static void
ul_line_length(struct options_type *options, unsigned long int ul)
{
  options->line_length = (unsigned int) ul;
}

static void
ul_stops(struct options_type *options, unsigned long int ul)
{
  options->stops = (unsigned int) ul;
}

static void
ul_buffer_size(struct options_type *options, unsigned long int ul)
{
  options->buffer_size = (unsigned int) ul;
}

static void
print_version(void)
{
  xprintf(VERSION_TEXT);    /* VERSION_TEXT is in config.h */
}

static void
version(struct options_type* options)
{
  (void) options;
  print_version();
  exit(EXIT_SUCCESS);
}

static void
help(struct options_type *options)
{
  const struct parameter_type *param = parameters;

  print_version();
  xprintf("Usage: %s [options] [filenames]\n", options->invocation);
  xprintf("Options:\n");

  for (;param->short_name; ++param)
  {
    /* Macros are in config.h */
    xprintf("  -%-*s or  --%-*s",
              HELP_DIVIDER1, param->short_name,
              HELP_DIVIDER2, param->long_name
           );

     if (param->ht)
         param->ht();
    else param->ht_ul(param->deflt);

         xprintf("\n");
  }

  xprintf("If no filenames are given, will read from standard input.\n");
  xprintf("If filenames are given, use a single - to read from standard input.\n");

  exit(EXIT_SUCCESS);
}

static void
ht_line_length(unsigned long int deflt)
{
  xprintf("Line length (default %u)", deflt);
}

static void
ht_always_hyphenate(void)
{
  xprintf("Fill up each line %s",
                (DEFAULT_ALWAYS_HYPHENATE) ?
                  "(default)" : "(switches off -k)");
}

static void
ht_line_break(void)
{
  xprintf("Line-break after whole words %s",
                DEFAULT_ALWAYS_HYPHENATE == 0 ?
                 "(default)" : "(switches off -a)");
}

static void
ht_stops(unsigned long int deflt)
{
  xprintf("Tab stop distance (default %u)", deflt);
}

static void
ht_buffer_size(unsigned long int deflt)
{
  xprintf("Buffer size (default %lu)", deflt);
}

static void
ht_help(void)
{
  xprintf("Prints out this help text");
}

static void
ht_version(void)
{
  xprintf("Version number");
}
