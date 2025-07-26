# Mental Vulkan Engine Makefile

# Compiler and flags
CC = clang
CFLAGS = -Wall -Wextra -std=c99
LDFLAGS = -L/opt/homebrew/lib -L/opt/homebrew/opt/molten-vk/lib
LIBS = -framework OpenGL -lglfw -lvulkan -lMoltenVK

# Directories
INCLUDES = -I/opt/homebrew/include -I/opt/homebrew/opt/molten-vk/include

# Source files
SRCDIR = .
ENGINE_DIR = engine
WM_DIR = engine/windowmanager
BACKENDS_DIR = engine/backends

SOURCES = \
	$(SRCDIR)/main.c \
	$(ENGINE_DIR)/engine.c \
	$(WM_DIR)/wm.c \
	$(BACKENDS_DIR)/mental_vk_support.c \
	$(BACKENDS_DIR)/mental_vk_instance.c \
	$(BACKENDS_DIR)/mental_vk_debug.c \
	$(BACKENDS_DIR)/mental_vk_queue_families.c \
	$(BACKENDS_DIR)/mental_vk_physical_device.c \
	$(BACKENDS_DIR)/mental_vk_logical_device.c \
	$(BACKENDS_DIR)/mental_vk_surface.c \
	$(BACKENDS_DIR)/mental_vk_swapchain

# Object files
OBJECTS = $(SOURCES:.c=.o)

# Target executable
TARGET = Mental

# Environment variables for Vulkan
export VK_LAYER_PATH = /opt/homebrew/share/vulkan/explicit_layer.d
export DYLD_LIBRARY_PATH = /opt/homebrew/lib

# Default target
all: $(TARGET)

# Build the executable
$(TARGET): $(OBJECTS)
	@echo "Linking $(TARGET)..."
	$(CC) $(OBJECTS) $(LDFLAGS) $(LIBS) -o $(TARGET)
	@echo "Build complete!"

# Compile source files
%.o: %.c
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Format all C and H files
format:
	@echo "Formatting all C and H files..."
	find . -name "*.c" -o -name "*.h" | xargs clang-format -i
	@echo "Formatting complete!"

# Check formatting without changing files
format-check:
	@echo "Checking code formatting..."
	@if find . -name "*.c" -o -name "*.h" | xargs clang-format --dry-run --Werror; then \
		echo "All files are properly formatted!"; \
	else \
		echo "Some files need formatting. Run 'make format' to fix."; \
		exit 1; \
	fi

# Clean build artifacts
clean:
	@echo "Cleaning build artifacts..."
	rm -f $(OBJECTS) $(TARGET)
	@echo "Clean complete!"

# Clean everything including logs
distclean: clean
	@echo "Cleaning everything..."
	rm -f log
	@echo "Distclean complete!"

# Install (optional)
install: $(TARGET)
	@echo "Installing $(TARGET)..."
	cp $(TARGET) /usr/local/bin/
	@echo "Install complete!"

# Uninstall
uninstall:
	@echo "Uninstalling $(TARGET)..."
	rm -f /usr/local/bin/$(TARGET)
	@echo "Uninstall complete!"

# Run the program
run: $(TARGET)
	@echo "Running $(TARGET)..."
	./$(TARGET)

# Debug build
debug: CFLAGS += -g -O0 -DDEBUG
debug: $(TARGET)

# Release build
release: CFLAGS += -O2 -DNDEBUG
release: $(TARGET)

# Show help
help:
	@echo "Available targets:"
	@echo "  all          - Build the executable (default)"
	@echo "  format       - Format all C and H files"
	@echo "  format-check - Check if files need formatting"
	@echo "  clean        - Remove object files and executable"
	@echo "  distclean    - Remove all build artifacts and logs"
	@echo "  install      - Install executable to /usr/local/bin"
	@echo "  uninstall    - Remove executable from /usr/local/bin"
	@echo "  run          - Build and run the program"
	@echo "  debug        - Build with debug symbols"
	@echo "  release      - Build optimized release version"
	@echo "  help         - Show this help message"

# Phony targets
.PHONY: all format format-check clean distclean install uninstall run debug release help 