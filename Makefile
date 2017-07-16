# Wrap v1.0 by Richard Cavell
# Copyright (c) 2017 Richard John Foster Cavell
# Makefile

vpath %.c Source
vpath %.h Include
vpath %.o Object
vpath %.txt . Source/Text

# Compilation variables
CC += -std=c89
# -Weverything gives a useless warning about padding in structs
CFLAGS += -Wall -Werror -Wextra -Wconversion -Wpedantic
CPPFLAGS += -I Include

.DEFAULT: all
.PHONY: all
all: wrap Readme.txt

# All object files are built using this command list
%.o:
	@echo "Compiling" $(<F)"..."
	$(COMPILE.c) $< $(OUTPUT_OPTION)

# Dependencies for each of the object files
Object/main.o:       main.c buffer.h file.h options.h output.h
Object/buffer.o:     buffer.c buffer.h options.h output.h
Object/file.o:       file.c file.h options.h output.h
Object/options.o:    options.c options.h config.h output.h
Object/output.o:     output.c output.h
Object/wrap.o:       wrap.c wrap.h buffer.c options.h

# Some header files #include other header files
buffer.h:            config.h options.h
file.h:              buffer.h options.h
wrap.h:              buffer.h options.h

# This is the final link step for the wrap program
wrap: main.o buffer.o file.o options.o output.o wrap.o
	@echo "Linking " $(@F)"..."
	$(LINK.c) $^ $(OUTPUT_OPTION)
	@echo "Executable" '"'$(@F)'"' "has been built."

# This builds the Readme.txt file
README_FILES = version.txt\
               readme_header.txt newline.txt\
               info.txt newline.txt\
               readme_footer.txt

Readme.txt: $(README_FILES)
	cat $+ > $@

# This removes intermediate files
OBJECT_FILES = Object/main.o\
 Object/buffer.o\
 Object/file.o\
 Object/options.o\
 Object/output.o\
 Object/wrap.o

.PHONY: clean
clean:
	@echo "Removing object files..."
	@$(RM) -v $(OBJECT_FILES) wrap
