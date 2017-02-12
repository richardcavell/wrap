# Wrap v1.0 by Richard Cavell
# January 2017
# Makefile

vpath %.c Source
vpath %.h Include
vpath %.o Object

CPPFLAGS = -I Include -std=c89 -Wall -Werror -pedantic -Wextra

wrap: main.o buffer.o error.o open_file.o options.o
	@echo "Linking..."
	@$(LINK.c) $^ $(OUTPUT_OPTION)
	@echo "Built:" $(@F)

Object/main.o: buffer.h config.h error.h open_file.h options.h
Object/main.o: main.c
	@echo "Building" $(@F)"..."
	@$(COMPILE.c) $< $(OUTPUT_OPTION)

Object/buffer.o: buffer.c buffer.h config.h error.h options.h
	@echo "Building" $(@F)"..."
	@$(COMPILE.c) $< $(OUTPUT_OPTION)

Object/error.o: error.c error.h
	@echo "Building" $(@F)"..."
	@$(COMPILE.c) $< $(OUTPUT_OPTION)

Object/open_file.o: open_file.c open_file.h options.h
	@echo "Building" $(@F)"..."
	@$(COMPILE.c) $< $(OUTPUT_OPTION)

Object/options.o: options.c options.h config.h error.h
	@echo "Building" $(@F)"..."
	@$(COMPILE.c) $< $(OUTPUT_OPTION)

.PHONY: clean

clean:
	@rm -f wrap Object/main.o Object/buffer.o Object/error.o \
Object/open_file.o Object/options.o
	@echo "All intermediate files have been deleted."
