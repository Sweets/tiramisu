
TARGET	=	tiramisu
SRC		:=	tiramisu.c callbacks.c format.c

CFLAGS	=	-Wall
LDFLAGS	=	$(shell pkg-config --libs --cflags glib-2.0 gio-2.0)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) $(SRC) -o $(TARGET)

clean:
	$(RM) ./tiramisu
