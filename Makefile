ifndef $(MAIN_DIR)
MAIN_DIR = $(HOME)/inetdoc
endif

# Liste des répertoires à traiter à partir de ce niveau
SUBDIRS = \
	images \
	articles \
	formations \
	guides \
	presentations \
	travaux_pratiques

# Type(s) de traitement
PROCESS = subdirs 

all: $(PROCESS)

include $(MAIN_DIR)/common/Makefile.Rules
