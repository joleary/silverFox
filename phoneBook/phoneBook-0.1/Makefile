#   Copyright 2010 Jamie O'Leary
#
#   This file is part of phoneBook.
#
 
PROGRAM = phoneBook

VERSION = 0.1

LOCAL_SRC_DIR = ./src

LOCAL_LIB_DIR = $(LOCAL_SRC_DIR)/lib

LOCAL_BUILD_DIR = ./build

CFLAGS += $(shell pkg-config --cflags gtk+-2.0)

LIBS += $(shell pkg-config --libs gtk+-2.0)

BIN = usr/bin

DATA = usr/share

all: pre-build $(PROGRAM)

phoneBook: main
	@echo -e "\nlinking targets\n"
	gcc -g $(CFLAGS) $(LIBS) $(LOCAL_LIB_DIR)/*.o -o $(LOCAL_BUILD_DIR)/$(PROGRAM)

pre-build:
	@echo -e "\nrunning pre-build target\n"
	@mkdir -p $(LOCAL_LIB_DIR)
	@mkdir -p $(LOCAL_BUILD_DIR)

main: parser
	@echo -e "\ncompiling main\n"
	gcc $(CFLAGS) $(LIBS) -g -c $(LOCAL_SRC_DIR)/phoneBook.c -o $(LOCAL_LIB_DIR)/main.o

parser:
	@echo -e "\ncompiling parser\n"
	gcc $(CFLAGS) $(LIBS) -g -c $(LOCAL_SRC_DIR)/phoneBookParser.c -o $(LOCAL_LIB_DIR)/phoneBookParser.o

.PHONY: clean
clean:
	@rm -f $(LOCAL_LIB_DIR)/*.o
	@rm -f $(LOCAL_BUILD_DIR)/$(PROGRAM)

install:
	install -d $(DESTDIR)/$(DATA)/applications
	install -d $(DESTDIR)/$(DATA)/$(PROGRAM)-$(VERSION)
	install -d $(DESTDIR)/$(BIN)
	install -m 0755 $(LOCAL_BUILD_DIR)/$(PROGRAM) $(DESTDIR)/$(DATA)/$(PROGRAM)-$(VERSION)/
	ln -s $(DESTDIR)/$(DATA)/$(PROGRAM)-$(VERSION)/$(PROGRAM) $(DESTDIR)/$(BIN)/$(PROGRAM)
