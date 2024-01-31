
CFLAGS=-Wall -g -Wno-comment -Wsign-compare -fPIC

clj:
	rm *.o 2> /dev/null || true
	rm lib/*.o 2> /dev/null || true
	rm lib/*.so 2> /dev/null || true
	
	g++ -c -fPIC *.cpp ${CFLAGS}

	g++ -o lib/clj.o *.o ${CFLAGS}
	g++ -o lib/clj.so *.o ${CFLAGS} -shared