buildTests : createBuildDir
	gcc -o tests/build/test tests/inst_live_test.c src/operations/inst_live.c  -l cmocka -L /usr/local/lib

createBuildDir :
	mkdir -p build

test :
	./tests/build/test

clean :
	rm -Rf build