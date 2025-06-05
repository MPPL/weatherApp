CXX = g++

eatherApp:
	mkdir -p build
	mkdir -p build/db
	g++ -lcurl -lsqlite3 -lssl -lcrypto -o build/eatherApp main.cpp src/*.cpp src/*.h