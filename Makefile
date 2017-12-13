LIBS = -lGLFW3
FRAMEWORKS = -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo
TARGET = doomengine
FLAGS = -g -std=c++14
CC = g++

all: $(TARGET)


$(TARGET): doomengine.o program.o utils.o screen.o input.o
	$(CC) $(FLAGS) $^ $(LIBS) $(FRAMEWORKS) -o $@
	
doomengine.o: doomengine.cpp main.hpp
	$(CC) $(FLAGS) -c $<

utils.o: utils.cpp utils.hpp main.hpp
	$(CC) $(FLAGS) -c $<

program.o: program.cpp program.hpp main.hpp
	$(CC) $(FLAGS) -c $<

screen.o: screen.cpp screen.hpp main.hpp
	$(CC) $(FLAGS) -c $<

input.o: input.cpp input.hpp main.hpp
	$(CC) $(FLAGS) -c $<

clean:
	rm -rf *.o $(TARGET)
