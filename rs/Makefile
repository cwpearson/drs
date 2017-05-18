
CXXFLAGS = -Wall -Wextra -Wpedantic -Weffc++ -std=c++11
OBJECTS=main.o rs_manager.o
TARGETS=main

all: main

%.o : %.cpp
	g++ $(CXXFLAGS) $^ -c -o $@

main: $(OBJECTS)
	g++ $(CXXFLAGS) $^ -o $@

clean:
	rm -f $(TARGETS)
	rm -f $(OBJECTS)
