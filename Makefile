CC=gcc
CFLAGS=-I./src/detector -fPIC -Wall
LDFLAGS=-shared -ldl

all: scarecrow.so test_target

scarecrow.so: src/detector/detector.c src/deception/deception.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

test_target: src/tests/test_target.c
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f scarecrow.so test_target 