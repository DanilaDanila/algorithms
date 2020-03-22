COMPILER=g++
LIBS=-lgtest -pthread
FLAGS=-std=c++14 -O3 -g -Wall -Wextra -Werror

all: main.cpp
	$(COMPILER) $(FLAGS) $(LIBS) -ggdb main.cpp -o test

clean:
	rm test
