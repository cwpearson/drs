
CXXFLAGS = -Wall -Wextra -Wpedantic -Weffc++ -std=c++11 -O2
OBJECTS=vecadd.o
TARGETS=vecadd
LIB = -lrt

all: $(TARGETS)

%.o : %.cpp
	g++ $(CXXFLAGS) $^ -c -o $@

vecadd: $(OBJECTS)
	g++ $(CXXFLAGS) $^ -o $@ $(LIB)

clean:
	rm -f $(TARGETS)
	rm -f $(OBJECTS)
