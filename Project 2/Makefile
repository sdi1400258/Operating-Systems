all: askisi2

askisi2: Algorithms.o Memory.o Simulator.o
	g++ -o askisi2 Algorithms.o Memory.o Simulator.o

Algorithms.o: Algorithms.cpp
	g++ -c Algorithms.cpp
	
Memory.o: Memory.cpp
	g++ -c Memory.cpp

Simulator.o: Simulator.cpp
	g++ -c Simulator.cpp

clean:
	rm -f *.o askisi2
