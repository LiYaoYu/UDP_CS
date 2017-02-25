CC := gcc

# for gdb debug
CFLAGS = -Wall -g -O0

# for performance
#CFLAGS = -Wall -O3

TARGET = udp.c console.c

client: main.c $(TARGET)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -rf client
