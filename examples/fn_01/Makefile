CC = gcc
SRCS = ../../slave/src/mb_slave.c ../../slave/src/mb_crc.c read_coil_status_01.c
INCS = ../../slave/inc/
EXE = read_coil_status_01

all:
	@ $(CC) -o $(EXE) $(SRCS) -I $(INCS)
	@ echo "Build Successfull."

clean:
	@ rm -f $(EXE)
	@ echo "Cleaned Successfull."