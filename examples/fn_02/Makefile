CC = gcc
SRCS = ../../slave/src/mb_slave.c ../../slave/src/mb_crc.c read_input_status_02.c
INCS = ../../slave/inc/
EXE = read_input_status_02

all:
	@ $(CC) -o $(EXE) $(SRCS) -I $(INCS)
	@ echo "Build Successfull."

clean:
	@ rm -f $(EXE)
	@ echo "Cleaned Successfull."