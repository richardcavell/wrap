wrap: Object/main.o Object/open_file.o Object/options.o
	$(CC) $^ -o $@ -std=c89 -Wall -Werror -pedantic

Object/main.o: Include/config.h Include/open_file.h Include/options.h
Object/main.o: Source/main.c
	$(COMPILE.c) $< -IInclude -o $@ -std=c89 -Wall -Werror -pedantic

Object/open_file.o: Source/open_file.c Include/open_file.h Include/options.h
	$(COMPILE.c) $< -IInclude -o $@ -std=c89 -Wall -Werror -pedantic

Object/options.o: Source/options.c Include/options.h Include/config.h
	$(COMPILE.c) $< -IInclude -o $@ -std=c89 -Wall -Werror -pedantic

.PHONY: clean

clean:
	@rm -f wrap Object/main.o Object/open_file.o Object/options.o
