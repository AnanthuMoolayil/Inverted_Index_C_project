CC = clang

SRC = $(wildcard *.c)	

TARGET = search_engine

all:
	$(CC) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET)