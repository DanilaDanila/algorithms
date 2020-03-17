COMPILER=g++
LIBS=-lgtest -pthread

all: main.o
	$(COMPILER) $(LIBS) main.o -o test

main.o: main.cpp biginteger.cpp
	$(COMPILER) -c main.cpp

clean:
	rm *.o biginteger
