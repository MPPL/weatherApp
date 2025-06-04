CXX = g++

eatherApp:
	mkdir -p build
	g++ -lsqlite3 -lssl -lcrypto -o build/eatherApp main.cpp src/*.cpp src/*.h