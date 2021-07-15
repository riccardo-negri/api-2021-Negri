LFLAGS = -DEVAL -Wall -Werror -std=gnu11 -O2 -pipe -static -s -o
RFLAGS = -lm
TEST_FILE = tests/open/input_3
compile_and_run: main.c
	@gcc  $(LFLAGS) main main.c $(RFLAGS)
	@#./main < $(TEST_FILE) > output
	@./tests.sh