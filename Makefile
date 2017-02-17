CC = gcc
TARGET = mmns
OBJECTS = main.o funclib.o
FLAGS = -Wall

all: $(TARGET)

%.o: %.c
	$(CC) -g -c -o $@ $< $(FLAGS)

clean: clean-obj clean-bin

clean-obj:
	rm -rf *.o
	
clean-bin:
	rm -rf $(TARGET)
	
$(TARGET): $(OBJECTS)
	$(CC) -g -o $(TARGET) $(OBJECTS)
