
TARGET	=	tiramisu
SRC		:=	tiramisu.c callbacks.c

CFLAGS	=	-Wall
LDFLAGS	=	$(shell pkg-config --libs --cflags glib-2.0 gio-2.0)

all: clean $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) $(SRC) -o $(TARGET)

clean:
	@[ -f ./tiramisu ] && $(RM) ./tiramisu
