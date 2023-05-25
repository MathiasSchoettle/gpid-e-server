CXX = g++
CXXFLAGS = -std=c++17 -Wall

# Directories
SRCDIR = src
BUILDDIR = build
TARGET = gpid-server

# Find all source files
SRCS := $(shell find $(SRCDIR) -name '*.cpp')
OBJS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SRCS:.cpp=.o))
DEPS := $(OBJS:.o=.d)

# Main target
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

# Include dependencies
-include $(DEPS)

# Compile .cpp to .o and generate dependency files
$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@
	$(CXX) -MM $(CXXFLAGS) $< > $(BUILDDIR)/$*.d

.PHONY: clean

clean:
	@rm -rf $(BUILDDIR) $(TARGET)