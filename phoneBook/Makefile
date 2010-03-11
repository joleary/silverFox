#   Copyright 2010 Jamie O'Leary
#
#   This file is part of phoneBook.
#
 
PROGRAM = phoneBook

LOCAL_LIB_DIR = ./lib

LOCAL_BUILD_DIR = ./build

CFLAGS += $(shell pkg-config --cflags gtk+-2.0)

LIBS += $(shell pkg-config --libs gtk+-2.0)

all: pre-build $(PROGRAM)

phoneBook: main
	@echo -e "\nlinking targets\n"
	gcc -g $(CFLAGS) $(LIBS) $(LOCAL_LIB_DIR)/*.o -o $(LOCAL_BUILD_DIR)/$(PROGRAM)

pre-build:
	@echo -e "\nrunning pre-build target\n"
	@mkdir -p build
	@mkdir -p lib

main: parser
	@echo -e "\ncompiling main\n"
	gcc $(CFLAGS) $(LIBS) -g -c phoneBook.c -o $(LOCAL_LIB_DIR)/main.o

parser:
	@echo -e "\ncompiling parser\n"
	gcc $(CFLAGS) $(LIBS) -g -c phoneBookParser.c -o $(LOCAL_LIB_DIR)/phoneBookParser.o

.PHONY: clean
clean:
	@rm -f $(LOCAL_LIB_DIR)/*.o
	@rm -f $(LOCAL_BUILD_DIR)/$(PROGRAM)
