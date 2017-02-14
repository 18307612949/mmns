CC = gcc
TARGET = mmns
OBJECTS = main.o funclib.o

all: $(TARGET)

%.o: %.c
	$(CC) -g -c -o $@ $<

clean: clean-obj clean-bin

clean-obj:
	rm -rf *.o
	
clean-bin:
	rm -rf $(TARGET)
	
$(TARGET): $(OBJECTS)
	$(CC) -g -o $(TARGET) $(OBJECTS)
