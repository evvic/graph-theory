# Specify the target executable
TARGET = arbitrage

# Include other directories
INC=-I/../../utilities

# Set the include path for the OpenSSL headers
INC += -I/usr/local/opt/openssl@3/include

UTI=../../utilities/

# Specify the source files
SOURCES = arbitrage.cpp $(UTI)c2c-edge.cpp $(UTI)Binance/c2c-json.cpp $(UTI)Binance/generate-signature.cpp \
		  $(UTI)Binance/c2c-buy.cpp $(UTI)graphviz.cpp bellman-ford-c2c.cpp \
		  $(UTI)read-keys.cpp ../../DFS/circular-arbitrage/dfs-arbitrage.cpp \
		  $(UTI)Binance/curl-scaffold.cpp

# Compiler flags
CXXFLAGS = -std=c++11
CXXFLAGS += $(INC)

# Linker flags
LDFLAGS += -L/usr/local/opt/openssl@3/lib -lssl -lcrypto
LDFLAGS += -ljsoncpp -lcurl

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
