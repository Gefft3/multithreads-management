CXX = g++ 

CXXFLAGS = -Wall -g

SOURCES = main.cpp

OBJECTS = $(SOURCES:.cpp=.o)

EXECUTABLE = program

all: $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJECTS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)


