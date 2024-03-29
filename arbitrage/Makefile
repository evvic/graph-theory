# Specify the target executable
TARGET = arbitrage

# Path to vcpkg installation
VCPKG_DIR = /usr/local/vcpkg

# Include other directories
INC=-I/../utilities

# Set the include path for the OpenSSL headers
INC += -I/usr/local/opt/openssl@3/include

# TBB library and include paths
# TBB_INCLUDE_DIR = $(VCPKG_DIR)/installed/x64-osx/include
# TBB_LIB_DIR = $(VCPKG_DIR)/installed/x64-osx/lib

# cppREST library linking
CPPRESTSDK_PATH = /usr/local/Cellar/cpprestsdk/2.10.18
CPPRESTSDK_INCLUDE_PATH = $(CPPRESTSDK_PATH)/include
CPPRESTSDK_LIB_PATH = $(CPPRESTSDK_PATH)/lib

CPPRESTSDK_LIBS = -lcpprest -lboost_system
CPPRESTSDK_INCLUDES = -I$(CPPRESTSDK_INCLUDE_PATH)

# Path for utilities
UTI=../utilities/

# Specify the source files
SOURCES = arbitrage.cpp $(UTI)edges/c2c-edge.cpp $(UTI)Binance/c2c-json.cpp $(UTI)http/generate-signature.cpp \
		  $(UTI)edges/unique-symbols.cpp $(UTI)Binance/c2c-buy.cpp $(UTI)graphviz.cpp \
		  $(UTI)read-keys.cpp ../DFS/circular-arbitrage/dfs-arbitrage.cpp $(UTI)Binance/convert.cpp \
		  $(UTI)http/http-scaffold.cpp ../DFS/circular-arbitrage/async-arbitrage.cpp $(UTI)Binance/quote-edge.cpp \
		  $(UTI)Binance/api-limits.cpp

# Compiler flags
CXXFLAGS = -std=c++11
CXXFLAGS += $(INC)
# CXXFLAGS += -I$(TBB_INCLUDE_DIR)
CXXFLAGS += $(CPPRESTSDK_INCLUDES)

# Linker flags
LDFLAGS += -L/usr/local/opt/openssl@3/lib -lssl -lcrypto
LDFLAGS += -ljsoncpp -lcurl
# LDFLAGS += -L$(TBB_LIB_DIR) -ltbb -ltbbmalloc
LDFLAGS += -L$(CPPRESTSDK_LIB_PATH) $(CPPRESTSDK_LIBS)

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
