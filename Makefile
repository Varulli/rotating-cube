SRCDIR=src
OBJDIR=obj

CC=gcc
OPTS=-O2 -g -Wall -Werror -Wno-error=unused-variable -Wno-error=unused-function
CFLAGS=$(OPTS)

NAME=rotating-cube

ifeq ($(OS),Windows_NT)
	RM = del /q
	RMDIR = del /s /q
	OBJECTS = $(OBJDIR)\*.o
	EXEC = $(NAME).exe
else
	RM = rm -f
	RMDIR = rm -rf
	OBJECTS = $(OBJDIR)/*.o
	EXEC = $(NAME)
endif

all: $(NAME)

$(NAME): $(OBJDIR)/$(NAME).o
	$(CC) $(CFLAGS) -o $@ $^

$(OBJDIR)/$(NAME).o: $(SRCDIR)/$(NAME).c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RMDIR) $(OBJECTS)
	$(RM) $(EXEC)