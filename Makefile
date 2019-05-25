LIBS =
INCLUDE=

ifeq ($(OS),Windows_NT)
	$(error Windows is not supported)
else
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S),Linux)
		INCLUDE += $(shell pkg-config --cflags glfw3) $(shell pkg-config --cflags gl)
		LIBS += $(shell pkg-config --libs glfw3) $(shell pkg-config --libs gl) -lpthread $(shell pkg-config --libs x11) -ldl -lXrandr -lXxf86vm -lXcursor -lXinerama
	endif
	ifeq ($(UNAME_S),Darwin)
		LIBS = -lGLFW3
		FRAMEWORKS = -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo
	endif
endif
TARGET = doomengine
FLAGS = -g -std=c++14
CC = g++

all: $(TARGET)


$(TARGET): doomengine.o program.o utils.o screen.o input.o
	$(CC) $(FLAGS) $^ $(LIBS) $(FRAMEWORKS) -o $@
	
doomengine.o: doomengine.cpp main.hpp
	$(CC) $(FLAGS) -c $(INCLUDE) $<

utils.o: utils.cpp utils.hpp main.hpp
	$(CC) $(FLAGS) -c $(INCLUDE) $<

program.o: program.cpp program.hpp main.hpp
	$(CC) $(FLAGS) -c $(INCLUDE) $<

screen.o: screen.cpp screen.hpp main.hpp
	$(CC) $(FLAGS) -c $(INCLUDE) $<

input.o: input.cpp input.hpp main.hpp
	$(CC) $(FLAGS) -c $(INCLUDE) $<

clean:
	rm -rf *.o $(TARGET)
