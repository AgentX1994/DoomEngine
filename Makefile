LIBS = -lGLFW3
FRAMEWORKS = -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo
TARGET = doomengine
FLAGS = -g -std=c++14
CC = g++

all: $(TARGET)


$(TARGET): doomengine.o program.o utils.o screen.o
	$(CC) $(FLAGS) $^ $(LIBS) $(FRAMEWORKS) -o $@
	
doomengine.o: doomengine.cpp
	$(CC) $(FLAGS) -c $<

utils.o: utils.cpp utils.hpp
	$(CC) $(FLAGS) -c $<

program.o: program.cpp program.hpp
	$(CC) $(FLAGS) -c $<

screen.o: screen.cpp screen.hpp
	$(CC) $(FLAGS) -c $<

clean:
	rm -rf *.o $(TARGET)
