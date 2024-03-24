smolz:
	g++ --std=c++17 -g -c ast.cpp
	g++ --std=c++17 -g -c lexer.cpp
	g++ --std=c++17 -g -c parser.cpp
	g++ --std=c++17 -g -c stringbuffer.cpp
	g++ --std=c++17 -g -c symboltable.cpp
	g++ --std=c++17 -g -c interpreter.cpp
	g++ --std=c++17 -g -c terp.cpp
	g++ *.o -o smolz
install:
	cp smolz /usr/local/bin

clean:
	rm *.o;
	rm smolz;