# include directory for Indexed Priority Queue data stucture
# INC=-I/../Utilities

floydworshall: fwMain.o floyd-worshall.o graphviz.o
	g++ fwMain.o floyd-worshall.o graphviz.o -o floydworshall

fwMain.o: fwMain.cpp
	g++ -c fwMain.cpp

graphviz.o: ../utilities/graphviz.cpp
	g++ -c ../utilities/graphviz.cpp

floyd-worshall.o: floyd-worshall.cpp
	g++ -c floyd-worshall.cpp

clean:
	rm *.o floydworshall