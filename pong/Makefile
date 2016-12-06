all: mp4

mp4: main.o
	g++ main.o -o mp4

main.o: main.cpp
	g++ -c main.cpp

clean:
	rm -rf *o mp4
