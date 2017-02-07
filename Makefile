COBJS=main.c funclib.c
CC=gcc
OUT=mmns

all:
	$(CC) -o $(OUT) $(COBJS)

debug:
	$(CC) -g -o $(OUT) $(COBJS)

clean:
	rm $(OUT)
