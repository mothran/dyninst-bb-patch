ifeq ($(DYNINST_ROOT),)
$(error DYNINST_ROOT is not set)
endif

CFLAGS=-O3
CXXFLAGS=-O3 -I$(DYNINST_ROOT)/include
PATCHER_LIBS=-L$(DYNINST_ROOT)/lib -lcommon -liberty -lboost_system -ldyninstAPI -lpthread

all: libruntime.so patcher

libruntime.so:
	$(CC) $(CFLAGS) -m64 -fPIC -shared -o libruntime.so libruntime.c -lpthread

patcher: libruntime.so
	$(CXX) $(CXXFLAGS) -o patcher patcher.cpp $(PATCHER_LIBS)

clean:
	rm -f patcher libruntime.so
