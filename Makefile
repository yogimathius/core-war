# Define directories
SRC_DIR := src
TESTS_DIR := tests
BUILD_DIR := build
TEST_BUILD_DIR := $(BUILD_DIR)/tests

# Compiler and flags
CC := gcc
CFLAGS := -Wall -Wextra -Werror
LDFLAGS := -lcmocka -L/usr/local/lib

# Define all test source files
TEST_SOURCES := $(wildcard $(TESTS_DIR)/*.c)

# Define all test executable names
TEST_EXECUTABLES := $(patsubst $(TESTS_DIR)/%.c,$(TEST_BUILD_DIR)/%,$(TEST_SOURCES))

# Default target to build all tests
all: createBuildDir $(TEST_EXECUTABLES)

# Create build directory if it doesn't exist
createBuildDir:
	mkdir -p $(BUILD_DIR)
	mkdir -p $(TEST_BUILD_DIR)

# Compile each test file
$(TEST_BUILD_DIR)/%: $(TESTS_DIR)/%.c
	$(CC) $(CFLAGS) -o $@ $< $(SRC_DIR)/*.c $(LDFLAGS)

# Run all tests
test: all
	@for test_exec in $(TEST_EXECUTABLES); do \
		$$test_exec; \
	done

# Clean build directory
clean:
	rm -rf $(BUILD_DIR)

.PHONY: all createBuildDir test clean
