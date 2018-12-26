# /*******************************************************************************
#
#                              tail, a trace tool
#
# *******************************************************************************/

all:
	gcc -O0 -g -o taildemo tail.c demo.c ../fifo/fifo.c -I. -I../fifo

clean:
	rm -f taildemo tail.o demo.o
