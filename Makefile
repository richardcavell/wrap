# Wrap v1.0 by Richard Cavell
# January 2017
# Makefile

vpath %.c Source
vpath %.h Include
vpath %.o Object

# flag -Weverything gives a useless warning about padding in structs
CPPFLAGS = -I Include -std=c89 -Wall -Werror -pedantic -Wextra

# All object files are built using this command list
%.o:
	@echo "Building" $(@F)"..."
	@$(COMPILE.c) $< $(OUTPUT_OPTION)

all: wrap

wrap: main.o buffer.o error.o open_file.o options.o
	@echo "Linking  "$(@F)"..."
	@$(LINK.c) $^ $(OUTPUT_OPTION)
	@echo "Executable" '"'$(@F)'"' "has been built."

Object/main.o:       main.c buffer.h config.h error.h open_file.h options.h
Object/buffer.o:     buffer.c buffer.h error.h options.h
Object/error.o:      error.c error.h
Object/open_file.o:  open_file.c open_file.h error.h options.h
Object/options.o:    options.c options.h config.h error.h

# Some header files #include other header files
buffer.h:            config.h options.h
open_file.h:         buffer.h options.h

.PHONY: all clean

clean:
	@rm -f wrap Object/main.o Object/buffer.o Object/error.o 
	@rm -f Object/open_file.o Object/options.o
	@echo "All intermediate files have been deleted."
