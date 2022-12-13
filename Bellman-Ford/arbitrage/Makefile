# Specify the target executable
TARGET = arbitrage

# Include other directories
INC=-I/../../utilities

UTI=../../utilities/

# Specify the source files
SOURCES = arbitrage.cpp $(UTI)c2c-edge.cpp $(UTI)c2c-json.cpp $(UTI)graphviz.cpp bellman-ford-c2c.cpp $(UTI)read-keys.cpp

# Compiler flags
CXXFLAGS = -std=c++11

# Linker flags
LDFLAGS += -ljsoncpp -lcurl
LD_LIBRARY_PATH += /usr/local/Cellar/jsoncpp

# Specify the C++ compiler
CXX = g++

# The remove command
RM = rf -f

# Build the target executable
$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(TARGET) $(LDFLAGS)

# Remove object files
clean: 
	rf -f *.o