CC = gcc
CFLAGS = -Wall -Wextra -Werror
LDFLAGS := -lcmocka -L/usr/local/lib

SRCDIR = ../src
TESTDIR = .
INCDIR = ../include
BUILDDIR = build
TEST_BUILDDIR = $(BUILDDIR)/tests

# Create necessary directories
$(shell mkdir -p $(TEST_BUILDDIR)/operations)

# Collect all source files
SOURCES = $(filter-out $(SRCDIR)/corewar.c, $(wildcard $(SRCDIR)/*.c))
OPERATIONS_SOURCES = $(wildcard $(SRCDIR)/operations/*.c)
TEST_SOURCES = $(wildcard $(TESTDIR)/*_test.c) $(wildcard $(TESTDIR)/operations/*_test.c)

# Generate object file paths
OBJECTS = $(SOURCES:$(SRCDIR)/%.c=$(BUILDDIR)/%.o)
OPERATIONS_OBJECTS = $(OPERATIONS_SOURCES:$(SRCDIR)/operations/%.c=$(TEST_BUILDDIR)/%.o)
TEST_OBJECTS = $(TEST_SOURCES:$(TESTDIR)/%.c=$(TEST_BUILDDIR)/%.o)

# Include directories
INCLUDES = -I$(INCDIR)

# Target for the test executable
TEST_TARGET = $(TEST_BUILDDIR)/run

all: $(TEST_TARGET)

$(TEST_TARGET): $(TEST_OBJECTS) $(OBJECTS) $(OPERATIONS_OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $<

$(TEST_BUILDDIR)/%.o: $(SRCDIR)/operations/%.c
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $<

$(TEST_BUILDDIR)/%.o: $(TESTDIR)/%.c
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $<

test: $(TEST_TARGET)
	./build/tests/run

clean:
	rm -f $(OBJECTS) $(OPERATIONS_OBJECTS) $(TEST_OBJECTS) $(TEST_TARGET)
