CC = gcc

CFLAGS = -W -Wall

TARGET = paint

OBJECTS = main.o event.o drawing.o ipc.o typo.o
	
all : $(TARGET)

$(TARGET) : $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^  -lX11 -lpthread
	rm *.o Drawing Answer

clean :
	rm *.o $(TARGET)
