# compiler
CC := g++
# include paths
INC := -I. -I./src
# libs
LIBS = -lm -lSDL -lGL -lGLU -lSDL_image
# objs,namess
OBJS := src/main.o src/eirin.o
NAME := build/gltest

# main
%.o: %.c %.cpp
	$(CC) -c -o $@ $< $(INC)

$(NAME): $(OBJS)
	$(CC) -o $@ $^ $(LIBS) -g

clean:
	rm $(OBJS)
