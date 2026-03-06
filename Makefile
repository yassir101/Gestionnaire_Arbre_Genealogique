CC      = gcc
CFLAGS  = -Wall -Wextra -pedantic -g
SRCDIR  = src
SRCS    = $(SRCDIR)/main.c \
          $(SRCDIR)/menus.c \
          $(SRCDIR)/gestion.c \
          $(SRCDIR)/getset.c \
          $(SRCDIR)/files.c \
          $(SRCDIR)/consulter.c \
          $(SRCDIR)/modif_pers.c \
          $(SRCDIR)/trouver_pers.c \
          $(SRCDIR)/determiner_lien.c
TARGET  = arbre_genealogique

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS)

clean:
	rm -f $(TARGET)

.PHONY: all clean
