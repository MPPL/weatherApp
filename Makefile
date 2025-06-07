CXX = g++ -std=c++20

eatherApp:
	mkdir -p build
	mkdir -p build/db
	${CXX} -lcurl -lsqlite3 -lssl -lcrypto -o build/eatherApp main.cpp src/*.cpp src/*.h