TARGET	= BLDCWindingCalculator
PREFIX	= /usr/local/bin
CC	= g++
CFLAGS	= -O3 -DNDEBUG -s -std=c++20

.PHONY: all clean install uninstall

all: $(TARGET)

clean:
	rm -rf $(TARGET) *.o
$(TARGET): $(TARGET).cpp
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).cpp
install:
	install $(TARGET) $(PREFIX)
uninstall:
	rm -rf $(PREFIX)/$(TARGET)
