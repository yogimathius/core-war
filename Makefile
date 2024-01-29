.PHONY: all clean test

all:
		$(MAKE) -C src

test:
		$(MAKE) -C tests

run_tests:
		./tests/build/tests/cmocka_test

clean:
		$(MAKE) -C src clean
		$(MAKE) -C tests clean
