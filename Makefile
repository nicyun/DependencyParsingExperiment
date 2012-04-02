CC=g++
CFLAGS=-c -Wall -g
LDFLAGS=
SOURCES=src/main.cpp src/DependencyPaser.cpp src/Environment.cpp src/Simulator.cpp src/WordAgent.cpp
OBJECTS=$(patsubst src/%.cpp, obj/%.o, $(SOURCES))
EXECUTABLE=bin/test

$(shell [ -d bin ] || mkdir -p bin)
$(shell [ -d obj ] || mkdir -p obj)

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

obj/%.o: src/%.cpp
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)
