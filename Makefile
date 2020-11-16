INCLUDE = include
OBJDIR = obj
BINDIR = bin
SRCDIRC = src/client
SRCDIRS = src/serveur

CC=gcc
CFLAGS=-Wall -I$(INCLUDE) -pedantic# -std=c11
#CFLAGS=-std=c99 -Wextra -Wall -Werror -pedantic
LDFLAGS=-lm

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

all: client serveur

client:print dependance
	@$(MAKE) $(EXECC)

serveur:print dependance
	@$(MAKE) $(EXECS)

$(EXECC):  $(OBJC)
	@$(CC) -o $@ $^ $(LDFLAGS)
	@mv $(SRCDIRC)/*.o obj


$(EXECS): $(OBJS)
	@$(CC) -o $@ $^ $(LDFLAGS)
	@mv $(SRCDIRS)/*.o obj

%.o: %.c
	@$(CC) -o  $@ -c $< $(CFLAGS)

#.PHONY permet d'éviter les problemes de dependance si un fichier s'appelle clean ou mrproper
.PHONY: clean mrproper

dependance:
	@mkdir -p $(BINDIR) $(OBJDIR)

print:
ifeq ($(DEBUG),yes)
	@echo "Générer en mode debug"
else
	@echo "Générer en mode release"
endif

#Permet de supprimer tout les fichiers .o
clean:
	@rm -rf $(OBJDIR)/*.o 

#Permet de supprimer tout ce qui à été générer
mrproper: clean
	@rm -rf $(OBJDIR) $(BINDIR) documentation/html documentation/latex $(SRCDIRC)/*.o $(SRCDIRS)/*.o

#Permet de générer la documentation
doc: include/*
	@doxygen documentation/TP2