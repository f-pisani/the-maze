# TheMaze Makefile
CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -O2 -fPIC
LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

# Qt5 includes (optional, only for Qt dependencies if any)
QT_CFLAGS = $(shell pkg-config --cflags Qt5Core 2>/dev/null)
QT_LIBS = $(shell pkg-config --libs Qt5Core 2>/dev/null)

# Directories
BUILD_DIR = build
SRC_DIR = .

# Source files
SOURCES = main.cpp TheMaze.cpp CMaze.cpp
OBJECTS = $(SOURCES:%.cpp=$(BUILD_DIR)/%.o)
EXECUTABLE = TheMaze

# Default target
all: $(BUILD_DIR) $(EXECUTABLE)

# Create build directory
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Link the executable
$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $@ $(LDFLAGS) $(QT_LIBS)

# Compile source files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) $(QT_CFLAGS) -c $< -o $@

# Dependencies
$(BUILD_DIR)/main.o: main.cpp TheMaze.hpp
$(BUILD_DIR)/TheMaze.o: TheMaze.cpp TheMaze.hpp CMaze.hpp
$(BUILD_DIR)/CMaze.o: CMaze.cpp CMaze.hpp

# Clean build artifacts
clean:
	rm -rf $(BUILD_DIR) $(EXECUTABLE)

# Install (optional)
install: $(EXECUTABLE)
	install -m 755 $(EXECUTABLE) /usr/local/bin/

# Run the game
run: $(EXECUTABLE)
	./$(EXECUTABLE)

# Debug build
debug: CXXFLAGS += -g -DDEBUG
debug: clean $(EXECUTABLE)

# Help
help:
	@echo "TheMaze Build System"
	@echo "==================="
	@echo "Targets:"
	@echo "  all     - Build the game (default)"
	@echo "  clean   - Remove build artifacts"
	@echo "  run     - Build and run the game"
	@echo "  debug   - Build with debug symbols"
	@echo "  install - Install to /usr/local/bin"
	@echo "  help    - Show this help message"

.PHONY: all clean install run debug help