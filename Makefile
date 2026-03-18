CC      = gcc
IN      = nosh.c
OUT     = nsh
LIBS 	= 
OPTS 	= -O0 -g -Wall -Wextra -pedantic

all:
	$(CC) -o$(OUT) $(LIBS) $(OPTS) $(IN)

run: all
	./$(OUT)

clean:
	rm $(OUT)
