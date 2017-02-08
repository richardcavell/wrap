# Wrap v1.0 by Richard Cavell
# January 2017

vpath %.c Source
vpath %.h Include
vpath %.o Object

CPPFLAGS = -I Include -std=c89 -Wall -Werror -pedantic

wrap: main.o open_file.o options.o
	$(CC) $^ $(OUTPUT_OPTION)

Object/main.o: config.h open_file.h options.h
Object/main.o: main.c
	$(COMPILE.c) $< $(OUTPUT_OPTION)

Object/open_file.o: open_file.c open_file.h options.h
	$(COMPILE.c) $< $(OUTPUT_OPTION)

Object/options.o: options.c options.h config.h
	$(COMPILE.c) $< $(OUTPUT_OPTION)

.PHONY: clean

clean:
	@rm -f wrap Object/main.o Object/open_file.o Object/options.o
