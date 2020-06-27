#
#	https://www.partow.net/programming/makefile/index.html
#

CC       := gcc
CCFLAGS  := -pedantic-errors -Wall -Wextra
VK_DIR   := /samba/public/code/ViewKit
LDFLAGS  := -L${VK_DIR}/lib -L/usr/X11R6/lib -lvk -lvkhelp -lXpm -lXmu -lXm -lXt -lX11 -lstdc++ ${VK_DIR}/lib/libvk.a ${VK_DIR}/lib/libvkhelp.a
BUILD    := ./build
OBJ_DIR  := $(BUILD)/objects
APP_DIR  := $(BUILD)/apps
TARGET   := program
INCLUDE  := -I${VK_DIR} -I/usr/X11R6/include
SRC      := $(wildcard *.cc) 
OBJECTS  := $(SRC:%.cc=$(OBJ_DIR)/%.o)

all: build $(APP_DIR)/$(TARGET)

$(OBJ_DIR)/%.o: %.cc
	@mkdir -p $(@D)
	$(CC) $(CCFLAGS) $(INCLUDE) -c $< -o $@ $(LDFLAGS)

$(APP_DIR)/$(TARGET): $(OBJECTS)
	@mkdir -p $(@D)
	$(CC) $(CCFLAGS) -o $(APP_DIR)/$(TARGET) $^ $(LDFLAGS)

.PHONY: all build clean debug release

build:
	@mkdir -p $(APP_DIR)
	@mkdir -p $(OBJ_DIR)

debug: CCFLAGS += -DDEBUG -g
debug: all

release: CCFLAGS += -O2
release: all

clean:
	-@rm -rvf $(OBJ_DIR)/*
	-@rm -rvf $(APP_DIR)/*