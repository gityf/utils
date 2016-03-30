CXX      = g++ -g -std=c++11
PB_HOME  = ./tools/protobuf-2.6.1/inbin/
PROTOC   = LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:$(PB_HOME)/lib $(PB_HOME)/bin/protoc
CXXFLAGS = -I$(PB_HOME)/include -I.
LDFLAGS  = -L$(PB_HOME)/lib -lprotobuf

all: demo.pb.h demo
demo.pb.h :
	$(PB_HOME)/bin/protoc --cpp_out=. ./demo.proto
demo :
	${CXX} ${CXXFLAGS} -o demo demo.cpp ${LDFLAGS}

clean:
	rm -rf demo *.pb.*
