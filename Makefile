all: tct nasa pdt

tct: TimeCode.cpp TimeCodeTests.cpp
	g++ -std=c++11 -Wall TimeCode.cpp TimeCodeTests.cpp -o tct

nasa: TimeCode.cpp NasaLaunchAnalysis.cpp
	g++ -std=c++11 -Wall TimeCode.cpp NasaLaunchAnalysis.cpp -o nasa

pdt: TimeCode.cpp PaintDryTimer.cpp
	g++ -std=c++11 -Wall TimeCode.cpp PaintDryTimer.cpp -o pdt

run: all
	./tct
	./nasa
	./pdt

clean:
	rm -f tct nasa pdt
