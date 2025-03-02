all:
	g++ -std=c++11 -Wall -o nla NasaLaunchAnalysis.cpp TimeCode.cpp
run:
	./nla
try: all run