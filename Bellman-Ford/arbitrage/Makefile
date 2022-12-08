# include directory for C2CEdge struct
INC=-I/../../utilities

arbitrage: arbitrage.o c2c-edge.o c2c-csv.o bellman-ford-c2c.o graphviz.o
	g++ arbitrage.o c2c-edge.o c2c-csv.o bellman-ford-c2c.o graphviz.o -o arbitrage

arbitrage.o: arbitrage.cpp
	g++ -c arbitrage.cpp

c2c-edge.o: ../../utilities/c2c-edge.cpp
	g++ -c ../../utilities/c2c-edge.cpp

c2c-csv.o: ../../utilities/c2c-csv.cpp
	g++ -c ../../utilities/c2c-csv.cpp

graphviz.o: ../../utilities/graphviz.cpp
	g++ -c ../../utilities/graphviz.cpp

bellman-ford-c2c.o: bellman-ford-c2c.cpp
	g++ -c bellman-ford-c2c.cpp

clean:
	rm *.o arbitrage