CC = clang

SRC_DIR = 1_source_files

SRC = $(wildcard $(SRC_DIR)/*.c)	

TARGET = search_engine

all:
	$(CC) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET)