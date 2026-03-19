CC      = gcc
IN      = nosh.c
OUT     = nsh
OPTS    = -O2 -Wall -Wextra -pedantic
OPTSDBG = -O0 -g -Wall -Wextra -pedantic

debug:
	$(CC) -o$(OUT) $(OPTSDBG) $(IN)

all:
	$(CC) -o$(OUT) $(OPTS) $(IN) 

run: all
	./$(OUT)

clean:
	rm $(OUT)
