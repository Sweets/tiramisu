
TARGET	=	tiramisu
SRC		:=	src/tiramisu.c src/events.c src/notification.c

PREFIX ?=	/usr/local

CFLAGS +=	-Wall -Wno-unused-value
IFLAGS  =	$(shell pkg-config --cflags glib-2.0 gio-2.0)
LFLAGS	=	$(shell pkg-config --libs glib-2.0 gio-2.0)

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(IFLAGS) $(SRC) $(LFLAGS) $(LDFLAGS) -o $(TARGET)

install: $(TARGET)
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	install $(TARGET) $(DESTDIR)$(PREFIX)/bin/$(TARGET)

clean:
	$(RM) ./tiramisu
