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

# Tests
yaml:
	$(CXX) -O3 -o tests/yaml ./tests/yaml.cpp /usr/lib/x86_64-linux-gnu/libyaml.a

rax-corruption:
	$(CC) -O3 -o tests/rax-corruption ./tests/rax-corruption.c

clean:
	rm -f patcher libruntime.so tests/rax-corruption tests/yaml
