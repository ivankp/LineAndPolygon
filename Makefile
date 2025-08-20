all: test

test: test.cpp LineAndPolygon.hpp
	g++ -std=c++17 -Wall -Wextra -fmax-errors=3 -O3 -I. $< -o $@
