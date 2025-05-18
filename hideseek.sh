#!/bin/bash
# hideseek - Command for hide and seek game theory operations

# Set base directory to the script location
BASE_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

# Define colors for output
GREEN='\033[0;32m'
RED='\033[0;31m'
BLUE='\033[0;34m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Print help information
function print_help {
    echo -e "${BLUE}Hide and Seek Game Theory Command${NC}"
    echo -e "${YELLOW}Usage:${NC}"
    echo -e "  hideseek <command> [options]"
    echo ""
    echo -e "${YELLOW}Commands:${NC}"
    echo -e "  ${GREEN}build${NC}       Build the project"
    echo -e "  ${GREEN}run${NC}         Run the game"
    echo -e "  ${GREEN}clean${NC}       Clean build files"
    echo -e "  ${GREEN}rebuild${NC}     Clean and rebuild the project"
    echo -e "  ${GREEN}help${NC}        Display this help message"
    echo ""
    echo -e "${YELLOW}Examples:${NC}"
    echo -e "  hideseek build"
    echo -e "  hideseek run"
    echo -e "  hideseek rebuild"
}

# Build the project
function build_project {
    echo -e "${BLUE}Building hide and seek project...${NC}"
    mkdir -p "$BASE_DIR/build"
    cd "$BASE_DIR/build" || exit
    cmake .. && cmake --build .
    if [ $? -eq 0 ]; then
        echo -e "${GREEN}Build successful${NC}"
        return 0
    else
        echo -e "${RED}Build failed${NC}"
        return 1
    fi
}

# Run the game
function run_game {
    if [ ! -f "$BASE_DIR/bin/hide_seek" ] && [ ! -f "$BASE_DIR/bin/hide_seek.exe" ]; then
        echo -e "${YELLOW}Executable not found. Building first...${NC}"
        build_project || return 1
    fi

    echo -e "${BLUE}Running hide and seek game...${NC}"

    # Save current directory
    CURRENT_DIR=$(pwd)

    # Change to project root directory before running
    cd "$BASE_DIR" || exit

    # Run the executable
    if [ -f "$BASE_DIR/bin/hide_seek" ]; then
        "$BASE_DIR/bin/hide_seek"
    elif [ -f "$BASE_DIR/bin/hide_seek.exe" ]; then
        "$BASE_DIR/bin/hide_seek.exe"
    else
        echo -e "${RED}Executable not found after build${NC}"
        cd "$CURRENT_DIR" || exit
        return 1
    fi

    # Return to original directory
    cd "$CURRENT_DIR" || exit
}

# Clean build files
function clean_project {
    echo -e "${BLUE}Cleaning project...${NC}"

    # Clean build directory
    if [ -d "$BASE_DIR/build" ]; then
        echo -e "${YELLOW}Cleaning build directory...${NC}"
        find "$BASE_DIR/build" -type f -not -name ".gitkeep" -delete
        find "$BASE_DIR/build" -type d -empty -delete
    fi

    # Clean bin directory
    if [ -d "$BASE_DIR/bin" ]; then
        echo -e "${YELLOW}Cleaning bin directory...${NC}"
        find "$BASE_DIR/bin" -type f -not -name ".gitkeep" -delete
    fi

    echo -e "${GREEN}Clean complete${NC}"
}

# Main command logic
case "$1" in
    build)
        build_project
        ;;
    run)
        run_game
        ;;
    clean)
        clean_project
        ;;
    rebuild)
        clean_project
        build_project
        ;;
    help|--help|-h)
        print_help
        ;;
    *)
        echo -e "${RED}Unknown command: $1${NC}"
        print_help
        exit 1
        ;;
esac
