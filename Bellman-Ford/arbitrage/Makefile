# include directory for C2CEdge struct
INC=-I/../../utilities

arbitrage: arbitrage.o c2c-edge.o c2c-json.o bellman-ford-c2c.o graphviz.o
	g++ -std=gnu++11 arbitrage.o c2c-edge.o c2c-json.o bellman-ford-c2c.o graphviz.o -o arbitrage -ljsoncpp -lcurl

arbitrage.o: arbitrage.cpp
	g++ -std=gnu++11 -c arbitrage.cpp -ljsoncpp -lcurl

c2c-edge.o: ../../utilities/c2c-edge.cpp
	g++ -std=gnu++11 -c ../../utilities/c2c-edge.cpp

c2c-json.o: ../../utilities/c2c-json.cpp
	g++ -std=gnu++11 -c ../../utilities/c2c-json.cpp -ljsoncpp -lcurl

graphviz.o: ../../utilities/graphviz.cpp
	g++ -std=gnu++11 -c ../../utilities/graphviz.cpp

bellman-ford-c2c.o: bellman-ford-c2c.cpp
	g++ -std=gnu++11 -c bellman-ford-c2c.cpp

clean:
	rm *.o arbitrage