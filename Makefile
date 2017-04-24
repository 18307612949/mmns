CC = gcc
TARGET = mmns
OBJECTS = main.o funclib.o sqlite3.o list.o
FLAGS = -Wall -pthread -ldl

all: $(TARGET)

%.o: %.c
	$(CC) -g -c -o $@ $< $(FLAGS)

clean: clean-obj clean-bin

clean-obj:
	rm -rf *.o
	
clean-bin:
	rm -rf $(TARGET)
	
$(TARGET): $(OBJECTS)
	$(CC) -g -o $(TARGET) $(OBJECTS) $(FLAGS)
