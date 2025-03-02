all:
	g++ -Wall NasaLaunchAnalysis.cpp -o NasaLaunchAnalysis
run:
	./NasaLaunchAnalysis
try: all run