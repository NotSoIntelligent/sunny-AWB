CC = gcc
CFLAGS = -Wall
OBJS = main.o bmpOps.o
OUT = imgProcess

all: $(OUT)

$(OUT): $(OBJS)
	$(CC) $(CFLAGS) -o $(OUT) $(OBJS)

clean:
	rm -f $(OBJS) $(OUT) ./*~
