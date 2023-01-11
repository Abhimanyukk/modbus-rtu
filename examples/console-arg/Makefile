CC = gcc
SRCS = ../../slave/src/mb_slave.c main.c
INCS = ../../slave/inc/
EXE = bin

all:
	@ $(CC) -o $(EXE) $(SRCS) -I $(INCS)
	@ echo "Build Successfull."

clean:
	@ rm -f $(EXE)
	@ echo "Cleaned Successfull."