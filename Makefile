TARGET = barber


CC = gcc


CFLAGS = -Wall -Wextra -pthread


SRCS = main.c


$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS)


run: $(TARGET)
	./$(TARGET)


clean:
	rm -f $(TARGET)
