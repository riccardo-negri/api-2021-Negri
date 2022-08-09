LFLAGS = -DEVAL -Wall -Werror -std=gnu11 -O2 -pipe -static -s -o
RFLAGS = -lm
compile_and_run: main.c
	@gcc  $(LFLAGS) main main.c $(RFLAGS)
	@./tests.sh
