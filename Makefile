# CC and CFLAGS are varilables
CC = g++
CFLAGS = -c
AR = ar
ARFLAGS = rcv
# -c option ask g++ to compile the source files, but do not link.
# -g option is for debugging version
# -O3 option is for optimized version
DBGFLAGS = -g -D_DEBUG_ON_
OPTFLAGS = -O3

all	: ./fp
	@echo -n ""

# optimized version
./fp	: SA.o main_opt.o tree.o net.o
			$(CC) $(OPTFLAGS) -o fp SA.o net.o tree.o main_opt.o
main_opt.o 	   	: main.cpp SA.h
			$(CC) $(CFLAGS) $< -o $@
SA.o	: SA.cpp  SA.h treeMgr.h nets.h terminal.h block.h
			$(CC) $(CFLAGS) $(OPTFLAGS) $< -o $@
net.o	: netMgr.cpp netMgr.h nets.h block.h terminal.h
			$(CC) $(CFLAGS) $(OPTFLAGS) $< -o $@
tree.o  : treeMgr.cpp treeMgr.h block.h contour.h
			$(CC) $(CFLAGS) $(OPTFLAGS) $< -o $@

# clean all the .o and executable files
clean:
		rm -rf *.o 
