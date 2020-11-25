all: clean pthread openmp java
.PHONY : all

clean:
	rm -f *.out *.class

pthread:
	g++ -Wall -Wextra -g -o pt.out -O2 Lab_PThread.cpp -lpthread

openmp:
	g++ -Wall -Wextra -g -o omp.out -O2 Lab_OpenMP.cpp -fopenmp

java:
	javac -g Lab_Jv.java
