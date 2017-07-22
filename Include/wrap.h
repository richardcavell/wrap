/* Wrap by Richard Cavell v1.0 */
/* Copyright (c) 2017 Richard John Foster Cavell */
/* wrap.h */

#ifndef WRAP_H
#define WRAP_H

#include <stdio.h>

#include "buffer.h"
#include "options.h"

extern int
wrap(FILE *fp, struct buffer_type *buffer,
     const struct options_type *options);

#endif
