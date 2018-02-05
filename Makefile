CC=g++

CFLAGS=-c -Wall -std=c++11 $(shell pkg-config --cflags gdk-pixbuf-2.0 gtkmm-3.0) -g
LDFLAGS=$(shell pkg-config --libs gdk-pixbuf-2.0 gtkmm-3.0) -pthread
SOURCES=$(shell find . -name '*.cpp')
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=chess

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	@echo Linking
	$(CC) -o $@ $(OBJECTS) $(LDFLAGS)

.cpp.o:
	@echo Compiling
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)

