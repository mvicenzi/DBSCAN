all: main
	
DBSCAN.o: DBSCAN.cpp StructDefinitions.h
	g++ -c DBSCAN.cpp

main: main.cpp DBSCAN.o StructDefinitions.h
	g++ -o main main.cpp DBSCAN.o

clear:
	rm *.o main
