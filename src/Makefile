$(info BCM28335 library must be used by a root user)
$(info run output binary file with sudo)

CC=gcc
CFLAGS=-I.

all: output 
output: main.o e_car_ctrl.o comm.o drv8343.o tmp275.o utilities.o ADS101x.o
	$(CC) main.o -l bcm2835 e_car_ctrl.o comm.o drv8343.o tmp275.o utilities.o ADS101x.o -o output
main.o: main.c e_car_ctrl.h comm.h drv8343.h tmp275.h utilities.h ADS101x.h
	$(CC)  -c main.c
e_car_ctrl.o: e_car_ctrl.c e_car_ctrl.h
	$(CC)  -c e_car_ctrl.c
comm.o: comm.c comm.h
	$(CC)  -c comm.c
drv8343.o: drv8343.c drv8343.h
	$(CC)  -c drv8343.c 
tmp275.o: tmp275.c tmp275.h
	$(CC)  -c tmp275.c 
utilities.o: utilities.c utilities.h
	$(CC)  -c utilities.c 
ADS101x.o: ADS101x.c ADS101x.h
	$(CC)  -c ADS101x.c 
clean:
	rm -rf *.o

