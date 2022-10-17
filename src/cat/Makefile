CC=gcc
CFLAGS=-Wall -Werror -Wextra #-fsanitize=address -std=c11 -pedantic
OS=$(shell uname)
ifeq ($(OS), Linux)
 LIBS=-lpthread -lrt -lm -lsubunit
 OPEN=xdg-open
 LEAKS=CK_FORK=no valgrind -s --tool=memcheck --leak-check=full --show-leak-kinds=all --show-reachable=yes ./test.out
else
 OPEN=open
 LEAKS=CK_FORK=no leaks --atExit -- ./test.out
endif

all: s21_cat cpplint cppcheck leaks

s21_cat: s21_cat.o cat.o
	$(CC) $(CFLAGS) s21_cat.o cat.o -o s21_cat

s21_cat.o: s21_cat.c
	$(CC) -c s21_cat.c

cat.o: cat.c
	$(CC) -c cat.c

test: access_test
	./test_cat.sh

access_test:
	chmod a+rwx test_cat.sh

val: access_test
	$(CC) -O0 -g s21_cat.c cat.c -o vgcheck
	valgrind ./test_cat.sh --leak-check=full

cpplint:
	python3 ../../materials/linters/cpplint.py --extensions=c *.c *.h

leaks: access_test
	leaks -atExit -- ./test_cat.sh

cppcheck:
	cppcheck *.c *.h

clean:
	rm -Rf *.o s21_cat *.txt vgcheck *.dSYM *.out

