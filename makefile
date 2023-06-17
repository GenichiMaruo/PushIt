CC = gcc
CFLAGS = -Wall -Wextra
LIBS = -lncurses
SRCDIR = src
INCDIR = include
BUILDDIR = build
OBJSUBDIRS := $(shell find $(SRCDIR) -type d -printf "$(BUILDDIR)/%P ")

SRCS := $(shell find $(SRCDIR) -name '*.c')
OBJS := $(patsubst $(SRCDIR)/%.c, $(BUILDDIR)/%.o, $(SRCS))
EXEC = pushit.exe

.PHONY: all clean force

all: $(EXEC)

$(EXEC): $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) -o $@ $(LIBS)
	@echo "Executable '$(EXEC)' generated."

$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(OBJSUBDIRS)
	@echo -n "Compiling $<... "
	@$(CC) $(CFLAGS) -I$(INCDIR) -c $< -o $@
	@echo "Done."

force: clean all

clean:
	@rm -f $(OBJS) $(EXEC)
	@echo "Cleaned."
