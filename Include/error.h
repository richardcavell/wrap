/* Wrap by Richard Cavell v1.0 */
/* Copyright (c) 2017 Richard John Foster Cavell */
/* error.h */

#ifndef ERROR_H
#define ERROR_H

extern void xprintf(const char *s, ...);   /* wrapper for printf() */
extern void xerror(const char *s, ...);    /* this returns to the caller */
extern void fail_msg(const char *s, ...);  /* this exits with EXIT_FAILURE */

#endif
