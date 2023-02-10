# Makefile

CC			= gcc
CPPFLAGS	= -Iinclude/ -Itest/
CFLAGS 		= -Wall -Werror -Wextra -std=c11 -Wno-unused-variable
LDFLAGS		=
LDLIBS		=

EXE = main.out

SRC	= $(wildcard *.c) $(wildcard src/*.c) $(wildcard src/*/*.c) $(wildcard test/*.c)
OBJ	= ${SRC:.c=.o}
DEP	= ${SRC:.c=.d}

-include $(DEP)

release: $(EXE)

debug: CFLAGS += -g -DDEBUG
debug: LDFLAGS += -ggdb3
debug: CPPFLAGS += -Itest/
debug: $(EXE)

$(EXE): $(OBJ)
	$(CC) $(LDFLAGS) $(LDLIBS) $(OBJ)  -lm -o $@

clean:
	$(RM) $(OBJ) 
	$(RM) $(DEP)
	$(RM) $(EXE)

# END
