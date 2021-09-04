TARGET	=	tiramisu
SRC	:=	src/tiramisu.c src/events.c src/notification.c

PREFIX	?=	/usr/local
INSTALL	=	install -Dm755
RM	?=	rm -f

CFLAGS	+=	-Wall -Wno-unused-value
IFLAGS	=	`pkg-config --cflags glib-2.0 gio-2.0`
LFLAGS	=	`pkg-config --libs glib-2.0 gio-2.0`

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(IFLAGS) $(SRC) $(LFLAGS) $(LDFLAGS) -o $(TARGET)

install: $(TARGET)
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	$(INSTALL) $(TARGET) $(DESTDIR)$(PREFIX)/bin/$(TARGET)

clean:
	$(RM) ./tiramisu
