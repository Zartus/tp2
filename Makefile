CC=gcc
CFLAGS=-Wall
#CFLAGS=-std=c99 -Wextra -Wall -Werror -pedantic 
LDFLAGS=-lm

OBJDIR = obj
BINDIR = bin
SRCDIRC = src/client
SRCDIRS = src/serveur

ifeq ($(DEBUG),yes)
	CFLAGS += -g
	LDFLAGS +=
else
	CFLAGS += -O3 -DNDEBUG
	LDFLAGS +=
endif

#client
EXECC=$(BINDIR)/clientExec
SRCC= $(wildcard $(SRCDIRC)/*.c)
OBJC= $(SRCC:.c=.o)

#serveur
EXECS=$(BINDIR)/serveurExec
SRCS= $(wildcard $(SRCDIRS)/*.c)
OBJS= $(SRCS:.c=.o)

all: 
ifeq ($(DEBUG),yes)
	@echo "Generating in debug mode"
else
	@echo "Generating in release mode"
endif
	mkdir -p $(OBJDIR)
	mkdir -p $(BINDIR)
	@$(MAKE) $(EXECC)
	@$(MAKE) $(EXECS)

$(EXECC): $(OBJC)
	@$(CC) -o $@ $^ $(LDFLAGS)
	mv $(SRCDIRC)/*.o $(OBJDIR)/

$(EXECS): $(OBJS)
	@$(CC) -o $@ $^ $(LDFLAGS)
	mv $(SRCDIRS)/*.o $(OBJDIR)/

%.o: %.c
	@$(CC) -o $@ -c $< $(CFLAGS)

.PHONY: clean mrproper

clean:
	@rm -rf $(OBJDIR)/*.o 

mrproper: clean
	@rm -rf $(OBJDIR) $(BINDIR) documentation/html

doc: $(SRCDIRC)/client.h $(SRCDIRS)/serveur.h
	@doxygen documentation/TP2
