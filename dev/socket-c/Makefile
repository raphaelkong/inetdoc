ifndef $(MAIN_DIR)
MAIN_DIR = $(HOME)/inetdoc
endif

# Liste des répertoires à traiter à partir de ce niveau
SUBDIRS = \
	files \
	images

# Type(s) de traitement
PROCESS = subdirs symlink xml2xhtml xml2pdf

SYMLINKS = \
        $(MAIN_DIR)/common/author.xml \
        $(MAIN_DIR)/common/legal.xml \
        $(MAIN_DIR)/common/inetdoc.urls.xml

XML_FILES = \
	socket-c.xml \
        author.xml \
        legal.xml \
	inetdoc.urls.xml \
	$(wildcard files/*.c) \
	$(wildcard images/*.odg)

all: $(PROCESS)

include $(MAIN_DIR)/common/Makefile.Rules
