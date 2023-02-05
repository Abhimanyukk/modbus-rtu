CC = gcc
SRCS = ../../slave/src/mb_slave.c ../../slave/src/mb_crc.c read_holding_register_03.c
INCS = ../../slave/inc/
EXE = read_holding_register_03

all:
	@ $(CC) -o $(EXE) $(SRCS) -I $(INCS)
	@ echo "Build Successfull."

clean:
	@ rm -f $(EXE)
	@ echo "Cleaned Successfull."