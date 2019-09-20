all:
	gcc -g -m32 -D_GNU_SOURCE thread.c queue.c switch.s simple_context_switch.c -o out
clean:
	rm -rf *.o *.a *.out