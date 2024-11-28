CC = gcc
CFLAGS = -Wall -g
SRCDIR = src
OBJDIR = build
SOURCES = $(wildcard $(SRCDIR)/*.c)
OBJECTS = $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
EXEC = c_quiz.out
all: $(EXEC)

$(EXEC): $(OBJECTS)
		$(CC) $(OBJECTS) -o $(EXEC)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
		$(CC) $(CFLAGS) -Iinclude -c $< -o $@

clean:
		rm -rf $(OBJDIR)/*.o $(EXEC)
.PHONY: all clean
