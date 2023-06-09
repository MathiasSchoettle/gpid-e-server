CC = g++
#CFLAGS = -Wall -Wextra -pedantic -std=c++11
CFLAGS = -Wall -w -pedantic -std=c++17
LDFLAGS = -lnsl -lstdc++ -lpaho-mqttpp3 -lpaho-mqtt3c -pthread -lpq

SRCDIR = src
OBJDIR = obj

SOURCES := $(wildcard $(SRCDIR)/*.cpp)
OBJECTS := $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SOURCES))
EXECUTABLE = gpid

.PHONY: all clean

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJDIR)
