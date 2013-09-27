CC=g++
CCFLAGS=-Wall -O

all: fweb

Site.o: Site.cpp Site.h 
	${CC} ${CCFLAGS} Site.cpp -c

Species.o: Species.cpp Species.h
	${CC} ${CCFLAGS} Species.cpp -c

Dynamic.o: Dynamic.cpp Dynamic.h 
	${CC} ${CCFLAGS} Dynamic.cpp -c

main.o: main.cpp Dynamic.h
	${CC} ${CCFLAGS} main.cpp -c

fweb: main.o Site.o Species.o Dynamic.o
	${CC} ${CCFLAGS} main.o Site.o Species.o Dynamic.o -o $@

clean: 
	rm -Rf *.o fweb *.rep *.log *.ini
