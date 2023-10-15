# Relative distance tree makefile.

CC = gcc

CCFLAGS = -O3
LINKLIBS = -lstdc++

all: driver java

driver: driver.cpp RDtree.hpp RDtree.cpp fileio.h fileio.cpp
	$(CC) $(CCFLAGS) -DBINARY_FILE_FORMAT -o driver driver.cpp RDtree.cpp fileio.cpp $(LINKLIBS)

java: RDtree.java RDclient.java
	javac -d . RDtree.java RDclient.java

clean:
	/bin/rm -f *.o
	/bin/rm -fr rdtree




