$(info BCM28335 library must be used by a root user)
$(info run output binary file with sudo)



all: output 
output: main.o e_car_ctrl.o
	gcc main.o -l bcm2835 e_car_ctrl.o -l bcm2835 -o output
main.o: main.c e_car_ctrl.h
	gcc -I . -c main.c
e_car_ctrl.o: e_car_ctrl.c e_car_ctrl.h
	gcc -I . -c e_car_ctrl.c
clean:
	rm -rf *.o

