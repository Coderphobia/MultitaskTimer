
test:
	gcc -Wall timer.c main.c -o  test

libtimer.a:
	
	gcc -c   timer.c
	ar  -cr  libtimr.a  timer.o

clean:
	rm -rf *.o  *.a  test
