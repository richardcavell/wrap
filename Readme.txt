Wrap (under development) by Richard Cavell
Copyright (c) 2017 Richard John Foster Cavell

A word-wrapping program.

This program handles all characters that are
mentioned in "The C Programming Language" by Kernighan
and Ritchie (2nd edition), including backspace,
tab, vertical tab, carriage return, and unprintable
characters.  It also handles extremely long lines.
It will only work with "narrow" characters, and
assumes that your output text is monospaced.

This program is written in standard C89, and should
work with any operating system (although you may
have to customize the build process for your OS).
It is licensed to you under the MIT License.

I give you permission to use the output of this
program for any worthwhile purpose. You need to
consider whether the input is encumbered by
intellectual property restrictions, and if it is,
that the output is probably also similarly
encumbered.

To build with GNU Make, type "make".  To run, run
the "wrap" program in the top-level directory.  To
see the options, run "wrap --help".
