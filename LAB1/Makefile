all: clean pthread openmp java
.PHONY : all

clean:
	rm -f *.out *.class

pthread: clean
	g++ -Wall -Wextra -g -o pt.out -O1 Lab_PThread.cpp -lpthread -std=c++17

openmp: clean
	g++ -Wall -Wextra -g -o omp.out -O1 Lab_OpenMP.cpp -fopenmp -std=c++17

java: clean
	javac -g Lab_Jv.java -encoding UTF8
