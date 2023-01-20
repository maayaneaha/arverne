# Makefile

CC			= gcc
CPPFLAGS	= -Iinclude/
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
debug: $(EXE)

$(EXE): $(OBJ)
	$(CC) $(LDFLAGS) $(LDLIBS) $(OBJ) -o $@

clean:
	$(RM) $(OBJ) 
	$(RM) $(DEP)
	$(RM) $(EXE)

# END
