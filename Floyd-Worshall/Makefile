# include directory for Indexed Priority Queue data stucture
# INC=-I/../Utilities

floydworshall: fwMain.o floyd-worshall.o graphviz.o
	g++ fwMain.o floyd-worshall.o graphviz.o -o floydworshall

fwMain.o: fwMain.cpp
	g++ -c fwMain.cpp

graphviz.o: ../Utilities/graphviz.cpp
	g++ -c ../Utilities/graphviz.cpp

floyd-worshall.o: floyd-worshall.cpp
	g++ -c floyd-worshall.cpp

clean:
	rm *.o floydworshall