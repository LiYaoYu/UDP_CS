CC := gcc

# for gdb debug
CFLAGS = -Wall -g -O0 -pthread

# for performance
#CFLAGS = -Wall -O3

TARGET = udp.c

server: main.c $(TARGET)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -rf server
