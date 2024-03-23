smolz:
	g++ --std=c++17 -c ast.cpp
	g++ --std=c++17 -c lexer.cpp
	g++ --std=c++17 -c parser.cpp
	g++ --std=c++17 -c stringbuffer.cpp
	g++ --std=c++17 -c symboltable.cpp
	g++ --std=c++17 -c interpreter.cpp
	g++ --std=c++17 -c terp.cpp
	g++ *.o -o smolz