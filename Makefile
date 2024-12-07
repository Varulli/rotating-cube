CC=gcc
OPTS=-O2 -Wall -Werror -Wno-error=unused-variable -Wno-error=unused-function
DEBUG=-g

SRCDIR=src
OBJDIR=obj
INCDIR=inc

INCLUDE=$(addprefix -I,$(INCDIR))
CFLAGS=$(OPTS) $(INCLUDE) $(DEBUG)

SOURCES=$(SRCDIR)/rotating-cube.c
ifeq ($(OS),Windows_NT)
	SOURCES+= $(SRCDIR)/init-win.c
	RM = del /q
	RMDIR = del /s /q
	EXEC = rotating-cube.exe
else
	SOURCES+= $(SRCDIR)/init-pos.c
	RM = rm -f
	RMDIR = rm -rf
	OBJECTS = $(OBJDIR)/*.o
	EXEC = rotating-cube
endif
OBJECTS=$(addprefix $(OBJDIR)/,$(notdir $(SOURCES:.c=.o)))

all: rotating-cube

rotating-cube: $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	$(RMDIR) $(OBJECTS)
	$(RM) $(EXEC)