CFLAGS=-std=c11 -Wall -Wextra
BUILDDIR=build

SOURCES := $(wildcard day*/day*.c)
TARGETS := $(addprefix $(BUILDDIR)/,$(notdir $(basename $(SOURCES))))

.PHONY: all
all: $(TARGETS)

$(BUILDDIR)/day1: day1/day1.c | $(BUILDDIR)
	$(CC) $(CFLAGS) $< -o $@

$(BUILDDIR)/day2: day2/day2.c | $(BUILDDIR)
	$(CC) $(CFLAGS) $< -o $@

$(BUILDDIR):
	mkdir -p $(BUILDDIR)

.PHONY: clean
clean:
	rm -rf $(BUILDDIR)
