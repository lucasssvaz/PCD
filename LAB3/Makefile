all: clean mpi
.PHONY : all

clean:
	rm -f *.out *.class

mpi: clean
	mpiCC -Wall -Wextra -g -o mpi.out -O1 Lab_PThread.cpp -std=c++17
