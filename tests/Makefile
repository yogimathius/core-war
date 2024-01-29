CC = gcc
CFLAGS = -Wall -Wextra -Werror
LDFLAGS := -lcmocka -L/usr/local/lib

SRCDIR = ../src
TESTDIR = .
INCDIR = ../include
BUILDDIR = build
TEST_BUILDDIR = $(BUILDDIR)/tests

$(shell mkdir -p $(TEST_BUILDDIR))
$(shell mkdir -p $(BUILDDIR)/operations)

# Collect all source files
SOURCES = $(filter-out $(SRCDIR)/corewar.c, $(wildcard $(SRCDIR)/*.c) $(wildcard $(SRCDIR)/operations/*.c))
TEST_SOURCES = $(wildcard $(TESTDIR)/*_test.c)

# Generate object file paths
OBJECTS = $(SOURCES:$(SRCDIR)/%.c=$(BUILDDIR)/%.o)
TEST_OBJECTS = $(TEST_SOURCES:$(TESTDIR)/%.c=$(TEST_BUILDDIR)/%.o)

# Include directories
INCLUDES = -I$(INCDIR)

# Target for the test executable
TEST_TARGET = $(TEST_BUILDDIR)/cmocka_test

all: $(TEST_TARGET)

$(TEST_TARGET): $(TEST_OBJECTS) $(OBJECTS)
		$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(BUILDDIR)/%.o: $(SRCDIR)/%.c
		$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $<

$(TEST_BUILDDIR)/%.o: $(TESTDIR)/%.c
		$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $<

test:
	./build/tests/cmocka_test

clean:
		rm -f $(OBJECTS) $(TEST_OBJECTS) $(TEST_TARGET)
