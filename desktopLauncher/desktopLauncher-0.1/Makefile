#   Copyright 2010 Jamie O'Leary
#
#   This file is part of desktopLauncher.
#
 
PROGRAM = desktopLauncher

VERSION = 0.1

LOCAL_SRC_DIR = ./src

LOCAL_LIB_DIR = ./src/lib

LOCAL_BUILD_DIR = ./build

CFLAGS += $(shell pkg-config --cflags gtk+-2.0 librsvg-2.0)

LIBS += $(shell pkg-config --libs gtk+-2.0 librsvg-2.0)

BIN = usr/bin

DATA = usr/share

all: pre-build $(PROGRAM)

desktopLauncher: main
	@echo -e "\nlinking targets\n"
	gcc -g $(CFLAGS) $(LIBS) $(LOCAL_LIB_DIR)/*.o -o $(LOCAL_BUILD_DIR)/$(PROGRAM)

pre-build:
	@echo -e "\nrunning pre-build target\n"
	@mkdir -p $(LOCAL_BUILD_DIR)
	@mkdir -p $(LOCAL_LIB_DIR)

main: launcher circleBox configParser optionWindow
	@echo -e "\ncompiling main\n"
	gcc $(CFLAGS) $(LIBS) -g -c $(LOCAL_SRC_DIR)/main.c -o $(LOCAL_LIB_DIR)/main.o

launcher:
	@echo -e "\ncompiling launcher\n"
	gcc $(CFLAGS) $(LIBS) -g -c $(LOCAL_SRC_DIR)/launcher.c -o $(LOCAL_LIB_DIR)/launcher.o

circleBox:
	@echo -e "\ncompiling circleBox\n"
	gcc $(CFLAGS) $(LIBS) -g -c $(LOCAL_SRC_DIR)/circleBox.c -o $(LOCAL_LIB_DIR)/circleBox.o

configParser:
	@echo -e "\ncompiling configParser\n"
	gcc $(CFLAGS) $(LIBS) -g -c $(LOCAL_SRC_DIR)/configParser.c -o $(LOCAL_LIB_DIR)/configParser.o

optionWindow:
	@echo -e "\ncompiling optionWindow\n"
	gcc $(CFLAGS) $(LIBS) -g -c $(LOCAL_SRC_DIR)/optionWindow.c -o $(LOCAL_LIB_DIR)/optionWindow.o

.PHONY: clean
clean:
	@rm -f $(LOCAL_LIB_DIR)/*.o
	@rm -f $(LOCAL_BUILD_DIR)/*

install:
	install -d $(DESTDIR)/$(DATA)/applications
	install -d $(DESTDIR)/$(DATA)/$(PROGRAM)-$(VERSION)
	install -d $(DESTDIR)/$(BIN)
	install -d $(DESTDIR)/etc/xdg/autostart
	install -m 0755 ./scripts/gmail $(DESTDIR)/$(BIN)
	install -m 0755 ./scripts/facebook $(DESTDIR)/$(BIN)
	install -m 0755 ./scripts/firefox-homeLauncher $(DESTDIR)/$(BIN)
	install -m 0644 ./$(PROGRAM).desktop $(DESTDIR)/etc/xdg/autostart
	install -m 0644 ./$(PROGRAM).desktop $(DESTDIR)/$(DATA)/applications
	install -m 0755 ./build/$(PROGRAM) $(DESTDIR)/$(DATA)/$(PROGRAM)-$(VERSION)/
	install -m 0644 ./config.xml $(DESTDIR)/$(DATA)/$(PROGRAM)-$(VERSION)/
	cp -r ./resources $(DESTDIR)/$(DATA)/$(PROGRAM)-$(VERSION)/
