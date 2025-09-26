CC      := gcc
CFLAGS  := -Wall -Wextra -O2 $(shell xml2-config --cflags)
LIBS    := $(shell xml2-config --libs)

# Исходники
SRCS    := parse_xml.c main.c

# Объектные файлы (по именам исходников)
OBJS    := $(SRCS:.c=.o)

TARGET  := nmap_open

.PHONY: all clean

all: $(TARGET)

# Правило для сборки итогового бинаря
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

# Паттерн для сборки объектного файла из .c
%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJS) $(TARGET)
