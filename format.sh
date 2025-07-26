#!/bin/bash

echo "Formatting all C and H files..."

# Format all .c and .h files recursively
find . -name "*.c" -o -name "*.h" | xargs clang-format -i

echo "Formatting complete!" 