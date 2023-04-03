# Makefile

CC = gcc
CFLAGS = `pkg-config --cflags gtk+-3.0` -Wall -O3 -std=c99
LDLIBS = -lm `pkg-config --libs gtk+-3.0` -export-dynamic
LDFLAGS = -rdynamic

all: interface

interface: interface.o

clean:
	${RM} *.o
	${RM} *.d
	${RM} interface
	${RM} *.bmp
# END
