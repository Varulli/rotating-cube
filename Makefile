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
	RM=del /q
	RMDIR=del /s /q
	EXEC=rotating-cube.exe
	MKDIR=-mkdir obj > nul 2>&1
else
	SOURCES+= $(SRCDIR)/init-pos.c
	SLASH=/
	RM=rm -f
	RMDIR=rm -rf
	EXEC=rotating-cube
	MKDIR=mkdir -p obj
endif
OBJECTS=$(addprefix $(OBJDIR)$(SLASH),$(notdir $(SOURCES:.c=.o)))

all: rotating-cube

rotating-cube: $(OBJECTS)
	$(CC) -o $@ $^ -lm

$(OBJDIR)$(SLASH)%.o: $(SRCDIR)/%.c
	$(MKDIR)
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	$(RMDIR) $(OBJECTS)
	$(RM) $(EXEC)