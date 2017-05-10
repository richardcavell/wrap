# Wrap v1.0 by Richard Cavell
# January-April 2017
# Makefile

vpath %.c Source
vpath %.h Include
vpath %.o Object
vpath %.txt . Source/Text

# -Weverything gives a useless warning about padding in structs
CFLAGS = -std=c89 -Wall -Werror -Wextra -pedantic
CPPFLAGS = -I Include

# All object files are built using this command list
%.o:
	@echo "Compiling" $(<F)"..."
	$(COMPILE.c) $< $(OUTPUT_OPTION)

.PHONY: all
all: wrap Readme.txt

wrap: main.o buffer.o error.o open_file.o options.o
	@echo "Linking " $(@F) "..."
	$(LINK.c) $^ $(OUTPUT_OPTION)
	@echo "Executable" '"'$(@F)'"' "has been built."

Object/main.o:       main.c buffer.h error.h open_file.h options.h
Object/buffer.o:     buffer.c buffer.h error.h options.h
Object/error.o:      error.c error.h
Object/open_file.o:  open_file.c open_file.h error.h options.h
Object/options.o:    options.c options.h config.h error.h

# Some header files #include other header files
buffer.h:            config.h options.h
open_file.h:         buffer.h options.h

Readme.txt: version.txt newline.txt readme_header.txt newline.txt\
            info.txt newline.txt readme_footer.txt
	cat $+ > $@

.PHONY: clean
clean:
	$(RM) wrap Object/main.o Object/buffer.o Object/error.o \
Object/open_file.o Object/options.o

