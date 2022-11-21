# include directory for Indexed Priority Queue data stucture
# INC=-I/../../utilities

connected-components: cc-main.o connected-components.o dfs.o
	g++ cc-main.o connected-components.o dfs.o -o connected-components

cc-main.o: cc-main.cpp
	g++ -c cc-main.cpp

# graphviz.o: ../../utilities/graphviz.cpp
# 	g++ -c ../../utilities/graphviz.cpp

connected-components.o: connected-components.cpp
	g++ -c connected-components.cpp

dfs.o: ../dfs.cpp
	g++ -c ../dfs.cpp



clean:
	rm *.o connected-components