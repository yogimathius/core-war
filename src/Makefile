CC = gcc
CFLAGS = -Wall -Wextra -Werror
LDFLAGS =

# Using CURDIR to set the SRCDIR
SRCDIR = $(CURDIR)
INCDIR = $(CURDIR)/../include
BUILDDIR = $(CURDIR)/../build
TARGET = $(BUILDDIR)/corewar

# Create necessary directories
$(shell mkdir -p $(BUILDDIR)/operations)

# Gather all source files recursively
SOURCES = $(wildcard $(SRCDIR)/*.c) $(wildcard $(SRCDIR)/operations/*.c)
OBJECTS = $(SOURCES:$(SRCDIR)/%.c=$(BUILDDIR)/%.o)
INCLUDES = -I$(INCDIR)

all: $(TARGET)

$(TARGET): $(OBJECTS)
		$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

$(BUILDDIR)/%.o: $(SRCDIR)/%.c
		$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $<

clean:
		rm -f $(OBJECTS) $(TARGET)
