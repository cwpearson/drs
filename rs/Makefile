
CXXFLAGS = -Wall -Weffc++ -std=c++11
OBJECTS=main.o
TARGETS=main

all: main

%.o : %.cpp
	g++ $(CXXFLAGS) $^ -c -o $@

main: $(OBJECTS)
	g++ $(CXXFLAGS) $^ -o $@

clean:
	rm -f $(TARGETS)
	rm -f $(OBJECTS)
