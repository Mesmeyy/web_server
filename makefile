all :server
.PHONY: clean

server:server_string.cpp server_epoll.cpp processpool.cpp 
	g++ -o server server_string.cpp server_epoll.cpp processpool.cpp  -std=c++11


clean:
	rm server
