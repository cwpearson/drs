
CXXFLAGS = -Wall -Wextra -Wpedantic -Weffc++ -std=c++11
OBJECTS=rs_object.o rs_ref.o rs_manager.o rs_node.o main.o
TARGETS=main

all: main

%.o : %.cpp
	g++ $(CXXFLAGS) $^ -c -o $@

main: $(OBJECTS)
	g++ $(CXXFLAGS) $^ -o $@

clean:
	rm -f $(TARGETS)
	rm -f $(OBJECTS)
