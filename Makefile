.PHONY: all clean test

all: src test run_tests

src:
		$(MAKE) -C src

test:
		$(MAKE) -C tests

run_tests:
		./tests/build/tests/run

clean:
		$(MAKE) -C src clean
		$(MAKE) -C tests clean
