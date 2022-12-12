# include directory for C2CEdge struct
INC=-I/../../utilities

arbitrage: arbitrage.o c2c-edge.o c2c-csv.o bellman-ford-c2c.o graphviz.o
	g++ -std=gnu++11 arbitrage.o c2c-edge.o c2c-csv.o bellman-ford-c2c.o graphviz.o -o arbitrage

arbitrage.o: arbitrage.cpp
	g++ -std=gnu++11 -c arbitrage.cpp

c2c-edge.o: ../../utilities/c2c-edge.cpp
	g++ -std=gnu++11 -c ../../utilities/c2c-edge.cpp

c2c-csv.o: ../../utilities/c2c-csv.cpp
	g++ -std=gnu++11 -c ../../utilities/c2c-csv.cpp

graphviz.o: ../../utilities/graphviz.cpp
	g++ -std=gnu++11 -c ../../utilities/graphviz.cpp

bellman-ford-c2c.o: bellman-ford-c2c.cpp
	g++ -std=gnu++11 -c bellman-ford-c2c.cpp

clean:
	rm *.o arbitrage