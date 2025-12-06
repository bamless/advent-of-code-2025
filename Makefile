CFLAGS  += -std=c11 -D_POSIX_C_SOURCE=199309L -Wall -Wextra
LDFLAGS  =-lm
BUILDDIR =build

SOURCES := $(wildcard day*/day*.c)
TARGETS := $(addprefix $(BUILDDIR)/,$(notdir $(basename $(SOURCES))))

.PHONY: all
all: $(TARGETS)

$(BUILDDIR)/day1: day1/day1.c | $(BUILDDIR)
	$(CC) $(CFLAGS) $(LDFLAGS) $< -o $@

$(BUILDDIR)/day2: day2/day2.c | $(BUILDDIR)
	$(CC) $(CFLAGS) $(LDFLAGS) $< -o $@

$(BUILDDIR)/day3: day3/day3.c | $(BUILDDIR)
	$(CC) $(CFLAGS) $(LDFLAGS) $< -o $@

$(BUILDDIR)/day4: day4/day4.c | $(BUILDDIR)
	$(CC) $(CFLAGS) $(LDFLAGS) $< -o $@

$(BUILDDIR)/day5: day5/day5.c | $(BUILDDIR)
	$(CC) $(CFLAGS) $(LDFLAGS) $< -o $@

$(BUILDDIR)/day6: day6/day6.c | $(BUILDDIR)
	$(CC) $(CFLAGS) $(LDFLAGS) $< -o $@

$(BUILDDIR):
	mkdir -p $(BUILDDIR)

.PHONY: clean
clean:
	rm -rf $(BUILDDIR)
