CC = gcc
INC_DIR := ../../slave/inc/
SRC_DIR = . ../../slave/src
EXE = bin

CFLAGS := -I $(INC_DIR)
SRCS := $(shell find $(SRC_DIR) -name '*.c')
OBJS := $(patsubst %.c, %.o, $(SRCS))

.PHONY: all clean

all: $(EXE)

%.o : %.c
	@$(CC) -c $(CFLAGS) $< -o $@
	@echo "Building... "$(shell basename $@)

$(EXE): $(OBJS)
	@$(CC) -o $@ $(OBJS) 
	@echo
	@echo "Build successfull... $@"
	@echo

clean:
	@$(RM) $(EXE) $(OBJS)
	@$(RM) -r $(OBJ_DIR)
	@echo "Cleaned..."
	@echo
