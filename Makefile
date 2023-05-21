# Makefile

CC			= gcc
CPPFLAGS	= -Iinclude/
CFLAGS 		= -Wall -Werror -Wextra -std=gnu99 -Wno-unused-variable -Wno-unused-function -Wno-unused-but-set-variable -Wno-unused-parameter `pkg-config --cflags gtk+-3.0`
LDLIBS = -lm `pkg-config --libs gtk+-3.0` -export-dynamic
LDFLAGS = -rdynamic

EXE = main.out

SRC	= $(wildcard *.c) $(wildcard src/*.c) $(wildcard src/*/*.c) $(wildcard test/*.c) $(wildcard interface/*.c)
OBJ	= ${SRC:.c=.o}
DEP	= ${SRC:.c=.d}

-include $(DEP)

release: $(EXE)

debug: CPPFLAGS += -Itest/
debug: CFLAGS += -g -DDEBUG
debug: LDFLAGS += -ggdb3
debug: $(EXE)

$(EXE): $(OBJ)
	$(CC) $(LDFLAGS) $(LDLIBS) $(OBJ)  -lm -o $@

clean:
	$(RM) $(OBJ)
	$(RM) $(DEP)
	$(RM) $(EXE)

# END
