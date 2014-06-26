OBJ_FILES = sharememory.o sharememorymessagequeue.o sharememorylogic.o sharememoryenvironment.o
CXX = g++
LD = g++

CXXFLAGS = -c -std=c++11 -Wall -g -ggdb -O0 -pthread
LDFLAGS = -pthread
DEF =

%.o : %.cpp
	$(CXX) $(CXXFLAGS) $(DEF) $^ -o $@

%.o : %.cxx
	$(CXX) $(CXXFLAGS) $(DEF) $^ -o $@

all : server client

server : $(OBJ_FILES) server.o sharememoryserver.o
	$(LD) $(LDFLAGS)  $^ -o $@

client : $(OBJ_FILES) client.o sharememoryclient.o
	$(LD) $(LDFLAGS)  $^ -o $@

.PHONY : clean
clean :
	rm *.o server client
