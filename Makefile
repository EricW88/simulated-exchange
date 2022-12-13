CXX      := g++
CXXFLAGS := -g -std=c++17 -Wall -Wextra -Wpedantic

run:
	$(CXX) $(CXXFLAGS) main.cpp -o ./Test
	./Test
