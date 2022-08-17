CXX = g++
CXXFLAGS = -Wall -std=c++2a -g -ggdb


all: accRejAgg.cpp segmentTree.hpp
	$(CXX) $(CXXFLAGS) accRejAgg.cpp -o accRejAgg


singleRankAgg: singleRankAgg.cpp segmentTree.hpp
	$(CXX) $(CXXFLAGS) singleRankAgg.cpp -o singleRankAgg

treeTest: treeTest.cpp segmentTree.hpp
	$(CXX) $(CXXFLAGS) treeTest.cpp -o test

oscilation: oscilation.cpp segmentTree.hpp
	$(CXX) $(CXXFLAGS) oscilation.cpp -o oscilation