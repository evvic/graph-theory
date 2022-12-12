# Specify the target executable
TARGET = example

# Specify the source files
SOURCES = curl3.cpp #foo.cpp bar.cpp

# Compiler flags
CXXFLAGS = -std=c++11

# Linker flags
LDFLAGS += -ljsoncpp -lcurl
LD_LIBRARY_PATH += /usr/local/Cellar/jsoncpp

# Specify the C++ compiler
CXX = g++

# Build the target executable
$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(TARGET) $(LDFLAGS)
