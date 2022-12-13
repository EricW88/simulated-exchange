CXX      := g++
CXXFLAGS := -g -std=c++17 -Wall -Wextra -Wpedantic

run: Order.hpp Client.hpp Level.hpp Exchange.hpp
	$(CXX) $(CXXFLAGS) Order.cpp Client.cpp Level.cpp Exchange.cpp Server.cpp -o ./Test
	./Test