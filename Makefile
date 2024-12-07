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
	SLASH=\\
	RM = del /q
	RMDIR = del /s /q
	EXEC = rotating-cube.exe
else
	SOURCES+= $(SRCDIR)/init-pos.c
	SLASH=/
	RM = rm -f
	RMDIR = rm -rf
	EXEC = rotating-cube
endif
OBJECTS=$(addprefix $(OBJDIR)$(SLASH),$(notdir $(SOURCES:.c=.o)))

all: rotating-cube

rotating-cube: $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

$(OBJDIR)$(SLASH)%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	$(RMDIR) $(OBJECTS)
	$(RM) $(EXEC)