#!/bin/bash

# TheMaze Game Script
# Universal script for setup, build, and run

set -e  # Exit on any error

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR"

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Print colored output
print_status() {
    echo -e "${BLUE}=== $1 ===${NC}"
}

print_success() {
    echo -e "${GREEN}✓ $1${NC}"
}

print_warning() {
    echo -e "${YELLOW}⚠ $1${NC}"
}

print_error() {
    echo -e "${RED}✗ $1${NC}"
}

# Check if dependencies are installed
check_dependencies() {
    local missing_deps=()
    
    if ! command -v g++ &> /dev/null; then
        missing_deps+=("g++")
    fi
    
    if ! command -v make &> /dev/null; then
        missing_deps+=("make")
    fi
    
    if ! pkg-config --exists sfml-graphics 2>/dev/null; then
        missing_deps+=("SFML")
    fi
    
    if ! pkg-config --exists Qt5Core 2>/dev/null; then
        missing_deps+=("Qt5")
    fi
    
    if [ ${#missing_deps[@]} -ne 0 ]; then
        return 1
    fi
    
    return 0
}

# Install dependencies
install_dependencies() {
    print_status "Installing Dependencies"
    
    if command -v apt-get &> /dev/null; then
        # Ubuntu/Debian
        print_status "Detected Ubuntu/Debian system"
        sudo apt-get update
        sudo apt-get install -y \
            build-essential \
            libsfml-dev \
            qtbase5-dev \
            libqt5core5a \
            pkg-config
            
    elif command -v dnf &> /dev/null; then
        # Fedora
        print_status "Detected Fedora system"
        sudo dnf install -y \
            gcc-c++ \
            make \
            SFML-devel \
            qt5-qtbase-devel \
            pkgconfig
            
    elif command -v pacman &> /dev/null; then
        # Arch Linux
        print_status "Detected Arch Linux system"
        sudo pacman -S --needed \
            base-devel \
            sfml \
            qt5-base \
            pkgconf
            
    else
        print_error "Unsupported distribution. Please install manually:"
        echo "- build-essential (gcc, g++, make)"
        echo "- SFML development libraries"
        echo "- Qt5 development libraries"
        echo "- pkg-config"
        exit 1
    fi
    
    print_success "Dependencies installed successfully!"
}

# Build the game
build_game() {
    print_status "Building TheMaze"
    
    # Check if required files exist
    if [ ! -f "Makefile" ]; then
        print_error "Makefile not found!"
        exit 1
    fi
    
    if [ ! -f "main.cpp" ] || [ ! -f "TheMaze.cpp" ] || [ ! -f "CMaze.cpp" ]; then
        print_error "Source files missing!"
        exit 1
    fi
    
    # Clean and build
    make clean
    if make; then
        print_success "Build successful!"
    else
        print_error "Build failed!"
        exit 1
    fi
}

# Run the game
run_game() {
    print_status "Running TheMaze"
    
    # Check if the executable exists
    if [ ! -f "TheMaze" ]; then
        print_error "TheMaze executable not found!"
        exit 1
    fi
    
    # Check if the maze file exists
    if [ ! -f "default.maze" ]; then
        print_error "default.maze file not found!"
        exit 1
    fi
    
    # Check if we're in a graphical environment
    if [ -z "$DISPLAY" ]; then
        print_error "No display found! This game requires a graphical environment."
        exit 1
    fi
    
    echo ""
    print_success "Starting TheMaze..."
    echo "Controls:"
    echo "  Arrow Keys - Move through the maze"
    echo "  ESC - Exit the game"
    echo ""
    echo "Press Ctrl+C to force quit if needed."
    echo ""
    
    # Run the game
    ./TheMaze
}

# Show usage
show_usage() {
    echo "TheMaze Game Script"
    echo ""
    echo "Usage: $0 [command]"
    echo ""
    echo "Commands:"
    echo "  setup   - Install dependencies only"
    echo "  build   - Build the game only"
    echo "  run     - Run the game only"
    echo "  clean   - Clean build artifacts"
    echo "  all     - Setup, build, and run (default)"
    echo "  help    - Show this help message"
    echo ""
    echo "Examples:"
    echo "  $0          # Setup, build, and run"
    echo "  $0 all      # Same as above"
    echo "  $0 setup    # Just install dependencies"
    echo "  $0 build    # Just build the game"
    echo "  $0 run      # Just run the game"
}

# Main logic
case "${1:-all}" in
    "setup")
        if check_dependencies; then
            print_success "All dependencies already installed!"
        else
            install_dependencies
        fi
        ;;
    "build")
        if ! check_dependencies; then
            print_error "Dependencies missing! Run: $0 setup"
            exit 1
        fi
        build_game
        ;;
    "run")
        if ! check_dependencies; then
            print_error "Dependencies missing! Run: $0 setup"
            exit 1
        fi
        run_game
        ;;
    "clean")
        print_status "Cleaning build artifacts"
        make clean
        print_success "Clean complete!"
        ;;
    "all")
        # Check and install dependencies if needed
        if ! check_dependencies; then
            print_warning "Dependencies missing, installing..."
            install_dependencies
        else
            print_success "Dependencies already installed!"
        fi
        
        # Build the game
        build_game
        
        # Run the game
        run_game
        ;;
    "help"|"-h"|"--help")
        show_usage
        ;;
    *)
        print_error "Unknown command: $1"
        show_usage
        exit 1
        ;;
esac