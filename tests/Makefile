buildTests : createBuildDir
	gcc -o build/test cmocka_test.c -l cmocka -L /usr/local/lib

createBuildDir :
	mkdir -p build

test :
	./build/test

clean :
	rm -Rf build