buildTests : createBuildDir
	gcc -o tests/build/test tests/cmocka_test.c -l cmocka -L /usr/local/lib

createBuildDir :
	mkdir -p build

test :
	./tests/build/test

clean :
	rm -Rf build