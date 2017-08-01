/* Wrap by Richard Cavell (under development) */
/* Copyright (c) 2017 Richard John Foster Cavell */
/* wrap.h */

#ifndef WRAP_H
#define WRAP_H

#include <stdio.h>

#include "buffer.h"
#include "options.h"

extern void
wrap(FILE *fp, struct buffer_type *buffer,
     const struct options_type *options);

#endif
