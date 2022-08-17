CXX = g++
CXXFLAGS = -Wall -std=c++2a -g -ggdb


all: accRejAgg.cpp segmentTree.hpp
	$(CXX) $(CXXFLAGS) accRejAgg.cpp -o accRejAgg


singleRankAgg: singleRankAgg.cpp segmentTree.hpp
	$(CXX) $(CXXFLAGS) singleRankAgg.cpp -o singleRankAgg

treeTest: treeTest.cpp segmentTree.hpp
	$(CXX) $(CXXFLAGS) treeTest.cpp -o test

singleRankFrag: singleRankFrag.cpp segmentTree.hpp
	$(CXX) $(CXXFLAGS) singleRankFrag.cpp -o singleRankFrag