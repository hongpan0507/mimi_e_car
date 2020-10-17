$(info BCM28335 library must be used by a root user)
$(info run output binary file with sudo)

CC=gcc
CFLAGS=-I.

all: output 
output: main.o e_car_ctrl.o comm.o
	$(CC) main.o -l bcm2835 e_car_ctrl.o -l bcm2835 comm.o -l bcm2835 -o output
main.o: main.c e_car_ctrl.h comm.h drv8343.h
	$(CC)  -c main.c
e_car_ctrl.o: e_car_ctrl.c e_car_ctrl.h
	$(CC)  -c e_car_ctrl.c
comm.o: comm.c comm.h
	$(CC)  -c comm.c
clean:
	rm -rf *.o

