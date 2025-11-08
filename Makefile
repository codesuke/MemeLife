# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++11 -Wall -Iinclude

# Source files
SOURCES = src/main.cpp src/Terminal.cpp src/AssetLoader.cpp src/Renderer.cpp src/Map.cpp src/Quiz.cpp src/Battle.cpp src/Save.cpp

# Object files
OBJECTS = $(SOURCES:.cpp=.o)

# Executable name
TARGET = MemeLife

# Default target
all: $(TARGET)

# Link object files to create executable
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS)

# Compile source files to object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	del /Q src\*.o $(TARGET).exe 2>nul || true

# Run the program
run: $(TARGET)
	.\$(TARGET)

# Convenience target to match requested invocation: make .\MemeLife.exe
.\\MemeLife.exe: $(TARGET)
	.\$(TARGET).exe

MemeLife.exe: $(TARGET)
	.\$(TARGET).exe

.PHONY: all clean run
