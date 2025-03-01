# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -Wextra -pedantic -std=c++20 -g -O3

# Source file
SRCS = wc.cpp

# Object file
OBJS = $(SRCS:.cpp=.o)

# Executable name
EXECS = wc

# Default target
all: $(EXECS)

# Compilation rule for wc
wc: wc.o
	$(CXX) $(CXXFLAGS) -o $@ $<

# Rule to compile the source file into an object file
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# Clean target to remove object files and executables
clean:
	rm -f $(OBJS) $(EXECS)

.PHONY: all clean
