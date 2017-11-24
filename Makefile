all:
	@rm -f log_replay
	g++ -std=c++11 -pthread main.cpp -o log_replay

install:
	cp -v log_replay ~/bin/
