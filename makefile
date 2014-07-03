OBJ_FILES_S = sharememory.o sharememorymessagequeue.o sharememorylogic.o sharememoryenvironment.o tablehumanoper.o config.o dbsql.o sharememoryserver.o
OBJ_FILES_C = sharememory.o sharememorymessagequeue.o sharememoryenvironment.o config.o 
CXX = g++
LD = g++

CXXFLAGS = -c -std=c++11 -Wall -g -ggdb -O0 -pthread
LDFLAGS = -pthread -lmysqlcppconn
DEF =

%.o : %.cpp
	$(CXX) $(CXXFLAGS) $(DEF) $^ -o $@

%.o : %.cxx
	$(CXX) $(CXXFLAGS) $(DEF) $^ -o $@

all : server client

server : $(OBJ_FILES_S) server.o dbserver.o
	$(LD) $(LDFLAGS)  $^ -o $@

client : $(OBJ_FILES_C) client.o sharememoryclient.o 
	$(LD) $(LDFLAGS)  $^ -o $@

.PHONY : clean
clean :
	rm *.o server client
