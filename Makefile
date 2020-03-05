# compiler
CC := gcc
# include paths
INC := -I. -I./src
# libs
LIBS = -lm -lSDL -lGL -lGLU -lSDL_image
# objs,namess
OBJS := src/main.o src/eirin.o
NAME := build/gltest

# main
%.o: %.c
	$(CC) -c -o $@ $<

$(NAME): $(OBJS)
	$(CC) -o $@ $^ $(LIBS)

clean:
	rm $(OBJS)
